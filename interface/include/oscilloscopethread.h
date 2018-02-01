#ifndef OSCILLOSCOPETHREAD_H
#define OSCILLOSCOPETHREAD_H

/*
 * 用于处理示波器数据
 * 数据的显示，通过主窗口指针实现
 * 主窗口对数据的操作，通过调用该类所提供的public函数实现
 * 一个定时器用于获取硬件数据
 * 一个定时器用于更新界面要显示的曲线，但控件的更新不能在这个线程做
 */

#include <QMainWindow>
#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <iostream>
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_layout.h"
#include "qwt_plot_magnifier.h"
#include "qwt_plot_panner.h"
#include "paintarea.h"
#include "showqueue.h"
#include "mrapi.h"
#include "joint.h"

#define GET_VALUE_INTEVAL   10  // 示波器采样周期（ms）
#define UPDATE_SHOW_INTEVAL 10  // 示波器刷新显示周期（ms）

class OscilloScopeThread : QObject
{
    Q_OBJECT
public:
    OscilloScopeThread(void * t, QObject *parent = 0); // t用于初始化成员变量view
    virtual ~OscilloScopeThread();
    PaintArea* paintArea;
    void changeTgPOS();
    void changeTgSPD();
    void changeTgCUR();
    void changeRlPOS();
    void changeRlSPD();
    void changeRlCUR();
protected:
    void * view; // 可以指向任一类的指针，进而执行该类中的函数
    QTimer* timerOfGetData;
    QTimer* timerOfShowData;
private:
    void setMask();
private slots:
    void getData();
    void showData();
};

#endif // OSCILLOSCOPETHREAD_H
