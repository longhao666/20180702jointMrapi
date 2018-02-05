#ifndef BOTTOM_H
#define BOTTOM_H


#include <QWidget>
#include "interfaglobal.h"

class QTimer;

namespace Ui {
class Bottom;
}

class Bottom : public QWidget
{
    Q_OBJECT
public:
//    JOINT_HANDLE joint;

public:
    Bottom(QWidget *parent = 0);
    ~Bottom();

public slots:
    void waitingForWidgetReady();
    void dealIDChanged(int newID);

signals:
    void cmbIDChanged(int ID);
    void cmbIDJoint();
    void signalRecoverBotton();


private:
    void updateEnableDriver();
    void updateWorkModePushButton();
    void updateConnected();
    void updateIfError();
    void updatecmbID();

private slots:
    void on_btnUpdateID_clicked();
    void on_enableDriverPushButton_clicked();
    void on_btnFlash_clicked();
    void on_btnLoad_clicked();
    void on_btnSave_clicked();
    void on_cmbID_currentIndexChanged(int index);
    void slotTimerBottomDone();

    void on_btnQuit_clicked();

private:
    Ui::Bottom *uiBottom;

    bool isCANInitialSucceed;
    QTimer *timerBottom;
};

#endif // BOTTOM_H
