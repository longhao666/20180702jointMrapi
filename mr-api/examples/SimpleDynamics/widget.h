#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "dyanamicsthread.h"

#include "joint.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pB_Connect_clicked();

    void update();

    void on_lE_Weight_editingFinished();

    void on_lE_Length_editingFinished();

    void on_lE_Kt_editingFinished();

    void on_lE_Coul_P_editingFinished();

    void on_lE_Coul_N_editingFinished();

    void on_lE_Vis_P_editingFinished();

    void on_lE_Vis_N_editingFinished();

    void on_lE_Coul_Coeff_editingFinished();

    void on_lE_CoulRise_Coeff_editingFinished();

    void on_lE_osc_freq_editingFinished();

    void on_lE_osc_amp_editingFinished();

    void on_lE_ZeroVel_editingFinished();

    void on_lE_CrtiAcc_editingFinished();

    void on_lE_Startup_TW_editingFinished();

    void on_lE_Stop_TW_editingFinished();

    void on_Widget_destroyed();

    void on_pB_Save2Flash_clicked();

    void on_lE_Coul_Temp_P_editingFinished();

    void on_lE_Coul_Temp_N_editingFinished();

    void on_lE_Vis_Temp_P_editingFinished();

    void on_lE_Vis_Temp_N_editingFinished();

private:
    Ui::Widget *ui;
    bool m_Connected;
    Joint* m_JointHandle;
    DyanamicsThread* m_Dynamics;
    QTimer *m_timer;
};

#endif // WIDGET_H
