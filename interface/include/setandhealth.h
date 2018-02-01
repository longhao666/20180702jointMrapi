#ifndef SETANDHEALTH_H
#define SETANDHEALTH_H

#include <QWidget>
#include <QTimer>
#include "mrapi.h"
#include "joint.h"

namespace Ui {
class SetAndHealth;
}

class SetAndHealth : public QWidget
{
    Q_OBJECT
public:
    JOINT_HANDLE joint;

public:
    SetAndHealth(QWidget *parent = 0);
    ~SetAndHealth();

public slots:
    void SetAndHealthIint(int ID);

signals:
    void IDChanged(int newID);
    void ZeroPositionSeted();

private:
    void set();
    void health();

private slots:
    void myTimerSlot();
    void on_IDPushButton_clicked();
    void on_setZeroPushButton_clicked();
    void on_ENonPPushButton_clicked();
    void on_clearErrorButton_clicked();
    void on_updateButton_clicked();

private:
    Ui::SetAndHealth *uiSetAndHealth;
    QTimer *timer;
};

#endif // SETANDHEALTH_H
