#ifndef MOVE_H
#define MOVE_H

#include <QWidget>
#include "interfaglobal.h"

class QTimer;
class QPushButton;

namespace Ui {
class Move;
}

class Move : public QWidget
{
    Q_OBJECT
public:
//    JOINT_HANDLE joint;

public:
    Move(QWidget *parent = 0);
    virtual ~Move();

public slots:
    void moveInit(int ID);
    void ClickStopButton();
    void slotRecoverButton();

private:
    void txtBiasChangeManualSlider();
    void workModeUpdatetxtBias();
    void setMoveValue(double value);
    void createSpeedButton();

private slots:
    void on_txtBias_editingFinished();
    void on_cmbWorkMode_currentIndexChanged(int index);
    void on_waveModeCombo_currentIndexChanged(int index);
    void on_frequencyLineEdit_editingFinished();
    void on_AmplitudeLineEdit_editingFinished();
    void on_manualSlider_valueChanged(int value);
    void slotTimeMoveDone();
    void on_confirmButton_clicked();
    void on_stopButton_clicked();
    void on_manualMin_editingFinished();
    void on_manualMax_editingFinished();


private:
    Ui::Move *uiMove;
    bool enableRun;
    QTimer *timerMove;
    double bias;
    double frequency;
    double amplitude;
    QPushButton *leftButton;
    QPushButton *rightButton;
};

#endif // MOVE_H
