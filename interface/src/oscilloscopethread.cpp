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
        iItem._Ptr->sq.MaxLength = 700; // grid一格100有7格
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
#if 0
//#define SCP_TAGCUR_L          0x92    //目标电流数据集   146
//#define SCP_TAGCUR_H          0x93    //目标电流数据集
//#define SCP_MEACUR_L          0x94    //实际电流数据集   148
//#define SCP_MEACUR_H          0x95    //实际电流数据集
//#define SCP_TAGSPD_L          0x96    //目标速度数据集   150
//#define SCP_TAGSPD_H          0x97    //目标速度数据集
//#define SCP_MEASPD_L          0x98    //实际速度数据集   152
//#define SCP_MEASPD_H          0x99    //实际速度数据集
//#define SCP_TAGPOS_L          0x9A    //目标位置数据集   154
//#define SCP_TAGPOS_H          0x9B    //目标位置数据集
//#define SCP_MEAPOS_L          0x9C    //实际位置数据集   156
//#define SCP_MEAPOS_H          0x9D    //实际位置数据集
            /*
             *  6次,依次为:  我曹,协议不对,上面这个定义才是正确的
             *  0: 触发方式： 0-上升沿， 1-下降沿， 2-连续采样， 3-用户触发 0x92 146 居然是给tgcur画波形的
             *  1: 目标电流数据集 0x96 150 居然是给tgspd画波形的
             *  2: 目标位置数据集 0x9a 154 居然是给tgpos画波形的
             *  3: 记录时间间隔（对 10kHZ 的分频值） 0x94 148 居然是给rlcur画波形的
             *  4: 目标速度数据集 0x98 152 居然是给respd画波形的
             *  5: 保留 0x9c 156 居然是给rlpos画波形的
             */
        qDebug() << "paintArea->showItems.size() = " << paintArea->showItems.size(); // 结果为 6
#endif
            for (unsigned int i = 0; i < paintArea->showItems.size(); i++) {
                if (paintArea->showItems[i].sq.MaxLength != 0 && paintArea->showItems[i].IsCheck) {
                    uint16_t data_L = 0;
                    uint16_t data_H = 0;
                    int32_t data32[2] = {0};
                    switch (paintArea->showItems[i].Item) {
                    case SCP_TAGCUR_L:
                        jointPollScope(m_joint, NULL, NULL, data32);
                        data_L = (uint16_t)(data32[0] & 0xffff);
                        data_H = (uint16_t)((data32[0] >> 16) & 0xffff);
//                        qDebug("SCP_TAGCUR_L");
                        break;
                    case SCP_MEACUR_L:
                        jointPollScope(m_joint, NULL, NULL, data32);
                        data_L = (uint16_t)(data32[1] & 0xffff);
                        data_H = (uint16_t)((data32[1] >> 16) & 0xffff);
//                        qDebug("SCP_MEACUR_L");
                        break;
                    case SCP_TAGSPD_L:
                        jointPollScope(m_joint, NULL, data32, NULL);
                        data_L = (uint16_t)(data32[0] & 0xffff);
                        data_H = (uint16_t)((data32[0] >> 16) & 0xffff);
//                        qDebug("SCP_TAGSPD_L");
                        break;
                    case SCP_MEASPD_L:
                        jointPollScope(m_joint, NULL, data32, NULL);
                        data_L = (uint16_t)(data32[1] & 0xffff);
                        data_H = (uint16_t)((data32[1] >> 16) & 0xffff);
//                        qDebug("SCP_MEASPD_L");
                        break;
                    case SCP_TAGPOS_L:
                        jointPollScope(m_joint, data32, NULL, NULL);
                        data_L = (uint16_t)(data32[0] & 0xffff);
                        data_H = (uint16_t)((data32[0] >> 16) & 0xffff);
//                        qDebug("SCP_TAGPOS_L");
                        break;
                    case SCP_MEAPOS_L:
                        jointPollScope(m_joint, data32, NULL, NULL);
                        data_L = (uint16_t)(data32[1] & 0xffff);
                        data_H = (uint16_t)((data32[1] >> 16) & 0xffff);
//                        qDebug("SCP_MEAPOS_L");
                        break;
                    default:
                        break;
                    }
                    double temp = data_L + (data_H * 65536);
#if 0
                    qDebug() << "paintArea->showItems[i].Item = " << paintArea->showItems[i].Item;
                    qDebug() << "paintArea->showItems[i].Item + 1 =" << paintArea->showItems[i].Item + 1;
                    qDebug() << "data_L = " << data_L << "data_H =" << data_H << "temp = " << temp;
#endif
                    switch (paintArea->showItems[i].Item) {
                    case SCP_TAGCUR_L://(showItems[0].Item)://TAG_CURRENT_L:
                    case SCP_MEACUR_L://showItems[3].Item://SYS_CURRENT_L://若曲线是电流
                        break;
                    case SCP_TAGSPD_L://showItems[1].Item://TAG_SPEED_L:
                    case SCP_MEASPD_L: {//showItems[4].Item://SYS_SPEED_L://若曲线是速度
//                        cout << temp << endl;
                        temp *= 60; // rpm
                        temp /= 65536; // rpm
//                        qDebug() << "sys_redu_ratio" << sys_redu_ratio;
                        temp /= sys_redu_ratio;
                        break;
                    }
                    case SCP_TAGPOS_L://showItems[2].Item://TAG_POSITION_L:
                    case SCP_MEAPOS_L://showItems[5].Item://SYS_POSITION_L://若曲线是位置
                        temp = temp * 360 / 65536; // degree（会在paintArea里进行转换显示）
//                        temp = data_L; // 位置要去掉圈数，只显示绝对编码器的数值 2017-3-28
                        break;
                    }
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
            }
        }
        switch (i) {
            case 0:JT->curveTgCUR->setSamples( points );break;
            case 1:JT->curveTgSPD->setSamples( points );break;
            case 2:JT->curveTgPOS->setSamples( points );break;
            case 3:JT->curveRlCUR->setSamples( points );break;
            case 4:JT->curveRlSPD->setSamples( points );break;
            case 5:JT->curveRlPOS->setSamples( points );break;
        }
    }
//    JT->ui->plot->replot();
//    JT->ui->plotSPD->replot();
//    JT->ui->plotPOS->replot();
}
