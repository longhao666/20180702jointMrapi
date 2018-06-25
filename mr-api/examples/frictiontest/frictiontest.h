#ifndef FRICTIONTEST_H
#define FRICTIONTEST_H

#include <QWidget>
#include "jointstate.h"
#include "mrapi.h"

namespace Ui {
class FrictionTest;
}

class FrictionTest : public QWidget
{
    Q_OBJECT

public:
    explicit FrictionTest(QWidget *parent = 0);
    ~FrictionTest();

private slots:
    void on_pB_ConnectJoint_clicked();

    void on_pB_StartPlot_clicked();

    void on_pB_StopPlot_clicked();

    void on_pB_Home_clicked();

    void on_pB_Go_clicked();

    void on_pushButton_clicked();

private:
    Ui::FrictionTest *ui;
    bool m_Connected;
    JOINT_HANDLE m_JointHandle;
    JointState *m_jointStateThread;
};

#endif // FRICTIONTEST_H
