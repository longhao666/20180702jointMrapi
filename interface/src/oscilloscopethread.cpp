#include <QDebug>
#include <QDateTime>
#include <vector>
#include "oscilloscopethread.h"
#include "oscilloscope.h"

using std::vector;

OscilloScopeThread::OscilloScopeThread(void *t, QObject *parent) :
    QObject(parent)
{
    view = t;
    //
    paintArea = new PaintArea();
    for (vector<ShowItem>::iterator iItem = paintArea->showItems.begin(); iItem != paintArea->showItems.end(); ++iItem) {
        iItem->sq.MaxLength = 700; // grid一格100有7格
    }
    for (unsigned int i = 0; i < paintArea->showItems.size(); i++) {
        paintArea->showItems[i].sq.FillZero();
    }
    //
    this->timerOfGetData = new QTimer(this);
    timerOfGetData->setTimerType(Qt::PreciseTimer);
    this->connect( timerOfGetData,
                   SIGNAL(timeout()),
                   this,
                   SLOT(getData()) ); // slotTimeOscilloScopeDone
    timerOfGetData->start(GET_VALUE_INTEVAL);
    //
    this->timerOfShowData = new QTimer(this);
    connect( timerOfShowData,
             SIGNAL(timeout()),
             this,
             SLOT(showData()) ); // updatePlot
    timerOfShowData->start(UPDATE_SHOW_INTEVAL);
}

OscilloScopeThread::~OscilloScopeThread()
{

}

void OscilloScopeThread::changeTgPOS()
{
    if (paintArea->showItems[2].IsCheck == true) { // showItems[2] is tgPOS
        paintArea->showItems[2].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGPOS;
    } else {
        paintArea->showItems[2].IsCheck = true;
        paintArea->Mask |= MASK_TAGPOS;
        paintArea->showItems[2].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::changeTgSPD()
{
    if (paintArea->showItems[1].IsCheck == true) { // showItems[1] is tgSPD
        paintArea->showItems[1].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGSPD;
    } else {
        paintArea->showItems[1].IsCheck = true;
        paintArea->Mask |= MASK_TAGSPD;
        paintArea->showItems[1].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::changeTgCUR()
{
    if (paintArea->showItems[0].IsCheck == true) { // showItems[0] is tgCUR
        paintArea->showItems[0].IsCheck = false;
        paintArea->Mask &= ~MASK_TAGCUR;
    } else {
        paintArea->showItems[0].IsCheck = true;
        paintArea->Mask |= MASK_TAGCUR;
        paintArea->showItems[0].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::changeRlPOS()
{
    if (paintArea->showItems[5].IsCheck == true) { // showItems[5] is rlPOS
        paintArea->showItems[5].IsCheck = false;
        paintArea->Mask &= ~MASK_MEAPOS;
    } else {
        paintArea->showItems[5].IsCheck = true;
        paintArea->Mask |= MASK_MEAPOS;
        paintArea->showItems[5].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::changeRlSPD()
{
    if (paintArea->showItems[4].IsCheck == true) { // showItems[4] is rlSPD
        paintArea->showItems[4].IsCheck = false;
        paintArea->Mask &= ~MASK_MEASPD;
    } else {
        paintArea->showItems[4].IsCheck = true;
        paintArea->Mask |= MASK_MEASPD;
        paintArea->showItems[4].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::changeRlCUR()
{
    if (paintArea->showItems[3].IsCheck == true) { // showItems[3] is rlCUR
        paintArea->showItems[3].IsCheck = false;
        paintArea->Mask &= ~MASK_MEACUR;
    } else {
        paintArea->showItems[3].IsCheck = true;
        paintArea->Mask |= MASK_MEACUR;
        paintArea->showItems[3].sq.FillZero();
    }
    setMask();
}

void OscilloScopeThread::setMask()
{
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (paintArea->Mask & 0xff00) >> 8 );
    data[0] = (uint8_t)( paintArea->Mask & 0xff );
    jointSet(SCP_MASK, 2, (Joint *)m_joint, (void *)&data, 50, NULL);
//    jointGet(SCP_MASK, 2, (Joint *)m_joint, NULL, 50, NULL);
}

void OscilloScopeThread::getData()
{
    if (paintArea->showItems.size() != 6) {
        return;
    }
    if (m_joint == NULL) {
        return;
    }
//    qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

    // 示波器绘制曲线使能开启
    if (paintArea->EnableScope) {
        // 示波器显示曲线用的计数量
        static int gatherCount = 0;
        // 间隔计数会从1开始判断
        ++gatherCount;
        // 按照Interval限定的间隔去执行
        if (gatherCount >= paintArea->Interval) {
            // 间隔计数清零
            gatherCount = 0;
            // 分别处理每个显示项
            // 显示项的显示队列不为0才向队列追加一个新值，该32位有符号值由2个16位有符号数组合而成
     for (unsigned int i = 0; i < paintArea->showItems.size(); i++) {
                if (paintArea->showItems[i].sq.MaxLength != 0 && paintArea->showItems[i].IsCheck) {
                    float f[2] = {0};
                    float cf = 0;
                    switch (paintArea->showItems[i].Item) {
                    case SCP_TAGCUR_L:
                        jointPollScope(m_joint, NULL, NULL, f);
                        cf = f[1];
                        break;
                    case SCP_MEACUR_L:
                        jointPollScope(m_joint, NULL, NULL, f);
                        cf = f[0];
                        break;
                    case SCP_TAGSPD_L:
                        jointPollScope(m_joint, NULL, f, NULL);
                        cf = f[1] / (sys_redu_ratio * 6);
                        break;
                    case SCP_MEASPD_L:
                        jointPollScope(m_joint, NULL, f, NULL);
                        cf = f[0] / (sys_redu_ratio * 6);
                        break;
                    case SCP_TAGPOS_L:
                        jointPollScope(m_joint, f, NULL, NULL);
                        cf = f[1];
                        break;
                    case SCP_MEAPOS_L:
                        jointPollScope(m_joint, f, NULL, NULL);
                        cf = f[0];
                        break;
                    default:
                        break;
                    }
                    double temp = cf;
                    //向队尾追加值
                    paintArea->showItems[i].sq.Append(temp);
                }
            }
        }
    }
}

void OscilloScopeThread::showData()
{
    OscilloScope * JT = static_cast<OscilloScope *>(view);
    //分别处理每条要测量的曲线
    for ( unsigned int i = 0; i < paintArea->showItems.size(); i++ ) {
        QPolygonF points;
        //若队列非空并且勾选显示
        if (!(paintArea->showItems[i].sq.IsEmpty()) && paintArea->showItems[i].IsCheck) {
            for (int j = 0; j < paintArea->showItems[i].sq.Count(); j++) {//分别处理每一坐标对
                double verticalValue1 = paintArea->showItems[i].sq.GetValue(j);
                points.append(QPointF(j, verticalValue1));
//                qDebug() << points.at(j).x() << " " << points.at(j).y();
            }
        }
        switch (i) {
//            case 0:JT->curveTgCUR->setSamples( points );break;
            case 1:
                JT->curveTgSPD->setSamples( points );
                break;
            case 2:JT->curveTgPOS->setSamples( points );break;
//            case 3:JT->curveRlCUR->setSamples( points );break;
            case 3:
                JT->curveRlCUR->axisY()->setRange(paintArea->showItems[i].sq.numberMin1,// + paintArea->showItems[i].sq.numberMin1/10,
                                                  paintArea->showItems[i].sq.numberMax1);// + paintArea->showItems[i].sq.numberMax1/10);
                JT->curveRlSplineSeries->replace(points);
                break;
            case 4:JT->curveRlSPD->setSamples( points );break;
            case 5:JT->curveRlPOS->setSamples( points );break;
        }
//        qDebug() << paintArea->showItems[i].sq.numberMin1 << paintArea->showItems[i].sq.numberMax1;
    }
}
