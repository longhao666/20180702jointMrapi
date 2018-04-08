#include <QPainter>
#include <QPointF>
#include <iostream>
#include <QPen>
#include "oscilloscope.h"

using std::vector;
using std::cout;
using std::endl;

// Gets the size of the Widget in the layout
void PaintArea::resizeEvent(QResizeEvent *event)
{
    return;
//    cout << "PaintArea::resizeEvent" << endl;
    QWidget::resizeEvent(event);
    this->setGeometry(this->rect());
    // Otherwise, the default value is always 100

//    //初始化数组pointX，当做要绘制曲线的X坐标，X坐标的值从0到Width - 1
//    pointX.clear();
//    for (int i = 0; i < this->width(); i++)
//    {
//        pointX.append(i);
//    }

    for (vector<ShowItem>::iterator iItem = this->showItems.begin(); iItem != this->showItems.end(); ++iItem) {
        iItem->sq.MaxLength = this->width();
    }
    for (unsigned int i = 0; i < showItems.size(); i++) {
        showItems[i].sq.FillZero();
    }
}

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent)
{
//    cout << this->geometry().width() << endl;
//    cout.flush();
//    cout << "PaintArea::PaintArea" << endl;

    setPalette(QPalette(Qt::gray));
    setAutoFillBackground(true);
    this->setHidden(true); // 避免在左上角出现
    Mask = 0;
    //另外的线程中需要引用
    Interval = 10; // Need additional initialization
    EnableScope = true; // 默认开启示波器
    // ShowItem and Mask would initialize in mainwindow_OscilloScope.cpp

    //电流、速度、位置值的单位转换比率
    speedRatio = 1;//60.0 / 65536;
    positionRatio = 360.0 / 65536;
    currentRatio = 1.0;
    //电流、速度、位置曲线沿Y轴的偏移值
    currentOffset = 0.0f;
    speedOffset = 0.0f; ;
    positionOffset = 0.0f;
    //电流、速度、位置值在Y轴上的缩放比例 // Initialize in MainWindow::OscilloScope()
    cr = 1;
    sr = 1;
    pr = 1;

    QPen pen;
    // pen.color().getRgb(r,g,b);
    pen.setBrush(QColor(TGCUR_RGB_R,TGCUR_RGB_G,TGCUR_RGB_B));
    ShowItem tempItem(false,SCP_TAGCUR_L,pen,this->width());
    showItems.push_back(tempItem);
    pen.setBrush(QColor(TGSPD_RGB_R,TGSPD_RGB_G,TGSPD_RGB_B));
    tempItem.Item = SCP_TAGSPD_L;tempItem.PenForThisItem = pen;
    showItems.push_back(tempItem);
    pen.setBrush(QColor(TGPOS_RGB_R,TGPOS_RGB_G,TGPOS_RGB_B));
    tempItem.Item = SCP_TAGPOS_L;tempItem.PenForThisItem = pen;
    showItems.push_back(tempItem);
    pen.setBrush(QColor(RLCUR_RGB_R,RLCUR_RGB_G,RLCUR_RGB_B));
    tempItem.Item = SCP_MEACUR_L;tempItem.PenForThisItem = pen;
    showItems.push_back(tempItem);
    pen.setBrush(QColor(RLSPD_RGB_R,RLSPD_RGB_G,RLSPD_RGB_B));
    tempItem.Item = SCP_MEASPD_L;tempItem.PenForThisItem = pen;
    showItems.push_back(tempItem);
    pen.setBrush(QColor(RLPOS_RGB_R,RLPOS_RGB_G,RLPOS_RGB_B));
    tempItem.Item = SCP_MEAPOS_L;tempItem.PenForThisItem = pen;
    showItems.push_back(tempItem);
//    cout << "paintArea() done" << endl;
}

PaintArea::~PaintArea()
{
}

void PaintArea::paintEvent(QPaintEvent *)
{
    return;
}
