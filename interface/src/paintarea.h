#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QMap>
#include <vector>
#include "showitem.h"

//示波器模块子索引地址定义
#define SCP_MASK    0x90 // 记录对象标志MASK
#define SCP_REC_TIM 0x91 // 记录时间间隔（对10kHZ的分频值）
#define SCP_TAGCUR_L    0x92 // 目标电流数据集
#define SCP_TAGCUR_H    0x93	//目标电流数据集
#define SCP_MEACUR_L    0x94	//实际电流数据集
#define SCP_MEACUR_H    0x95	//实际电流数据集
#define SCP_TAGSPD_L    0x96	//目标速度数据集
#define SCP_TAGSPD_H    0x97	//目标速度数据集
#define SCP_MEASPD_L    0x98	//实际速度数据集
#define SCP_MEASPD_H    0x99	//实际速度数据集
#define SCP_TAGPOS_L    0x9A	//目标位置数据集
#define SCP_TAGPOS_H    0x9B	//目标位置数据集
#define SCP_MEAPOS_L    0x9C	//实际位置数据集
#define SCP_MEAPOS_H    0x9D	//实际位置数据集

#define MASK_TAGCUR 0x01 // 记录目标电流MASK
#define MASK_MEACUR 0x02 // 记录实际电流MASK
#define MASK_TAGSPD 0x04 // 记录目标速度MASK
#define MASK_MEASPD 0x08 // 记录实际速度MASK
#define MASK_TAGPOS 0x10 // 记录目标位置MASK
#define MASK_MEAPOS 0x20 // 记录实际位置MASK

#define TGPOS_RGB_R 218 // Fandango 218, 73, 128
#define TGPOS_RGB_G 73
#define TGPOS_RGB_B 128
#define TGSPD_RGB_R 131 // Greenery 131, 177, 78
#define TGSPD_RGB_G 177
#define TGSPD_RGB_B 78
#define TGCUR_RGB_R 67  // Dazzling Blue 67, 73, 157
#define TGCUR_RGB_G 73
#define TGCUR_RGB_B 157
#define RLPOS_RGB_R 237 // Prism Pink 237, 158, 189
#define RLPOS_RGB_G 158
#define RLPOS_RGB_B 189
#define RLSPD_RGB_R 247 // Blazing Yellow 247, 235, 55
#define RLSPD_RGB_G 235
#define RLSPD_RGB_B 55
#define RLCUR_RGB_R 237 // Brilliant White 237, 239, 252
#define RLCUR_RGB_G 239
#define RLCUR_RGB_B 252

#define SCOPEENABLE_RGB_R   131 // Greenery 131, 177, 78
#define SCOPEENABLE_RGB_G   177
#define SCOPEENABLE_RGB_B   78
#define SCOPEDISABLE_RGB_R  247 // Blazing Yellow 247, 235, 55
#define SCOPEDISABLE_RGB_G  235
#define SCOPEDISABLE_RGB_B  55

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    explicit PaintArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~PaintArea();

    std::vector<ShowItem> showItems;
    unsigned int Mask;
    unsigned int ScanFrequency;
    int Interval; // The frequency at which new data is acquired
    bool EnableScope;

//    //存储X坐标
//    QVector<float> pointX;
//    float pointX[1024];

    //电流、速度、位置值的单位转换比率
    double speedRatio;
    double positionRatio;
    double currentRatio;

    //电流、速度、位置值在Y轴上的缩放比例
    double cr;
    double sr;
    double pr;

    //电流、速度、位置曲线沿Y轴的偏移值
    double currentOffset;
    double speedOffset;
    double positionOffset;

signals:

public slots:

private:
//    QPen pen; // creates a default pen
//    QPoint point1;
//    QPoint point2;
//    QPainter * p1;
    virtual void resizeEvent(QResizeEvent * event);

};

#endif // PAINTAREA_H
