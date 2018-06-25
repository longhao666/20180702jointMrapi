#include "frictiontest.h"
#include "ui_frictiontest.h"
#include <QDebug>
#include <Windows.h>

FrictionTest::FrictionTest(QWidget *parent) :
    QWidget(parent),m_Connected(false),
    ui(new Ui::FrictionTest),
    m_JointHandle((void*)0)
{
    startMaster("pcanusb1", MASTER(0));

    ui->setupUi(this);

    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(1, 50);
    ui->lE_Jointid->setValidator(aIntValidator);
    ui->pB_StartPlot->setDisabled(true);
    ui->pB_StopPlot->setDisabled(true);
}

FrictionTest::~FrictionTest()
{
    delete ui;
}

void FrictionTest::on_pB_ConnectJoint_clicked()
{
    uint16_t jointid = ui->lE_Jointid->text().toInt();
    if (!m_Connected) {
        m_JointHandle = jointUp(jointid, MASTER(0));
        if (m_JointHandle) {
            if (jointSetMode(m_JointHandle, joint_position, 1000, NULL) == MR_ERROR_ACK1) {
                qDebug() << "CONNECTED";
            }
            ui->pB_ConnectJoint->setText("CONNECTED");
            ui->pB_ConnectJoint->setStyleSheet("background-color: rgb(0, 255, 0);");
            ui->pB_ConnectJoint->update();
            ui->pB_StartPlot->setEnabled(true);
            m_Connected = true;
        } else {
            qDebug() << "Connect FAILED";
            m_Connected = false;
        }
    } else {
        jointDown(m_JointHandle);
        qDebug() << "DISCONNECTED";
        ui->pB_ConnectJoint->setText("Connect");
        ui->pB_ConnectJoint->setStyleSheet("background-color: rgb(240, 240, 240);");
        ui->pB_ConnectJoint->update();
        ui->pB_StartPlot->setEnabled(false);
        m_Connected = false;
    }
}

void FrictionTest::on_pB_StartPlot_clicked()
{
    float pos = 0.0f, vel, curr;
    jointSetPosition(m_JointHandle, 15.0f, -1, NULL);

    while(fabs(pos -15.0f) > 0.01f) {
        jointPoll(m_JointHandle, &pos, &vel, &curr);
        qDebug("pos: %f\n", pos);
        Sleep(1);
    }

    if (jointSetMode(m_JointHandle, joint_cyclesync, 1000, NULL) == MR_ERROR_ACK1) {
        m_jointStateThread = new JointState(this, m_JointHandle);
        m_jointStateThread->start();
        ui->pB_StopPlot->setEnabled(true);
    }
}

void FrictionTest::on_pB_StopPlot_clicked()
{
    float pos = 0.0f, vel, curr;
    m_jointStateThread->stop();
    m_jointStateThread->wait();

    if (jointSetMode(m_JointHandle, joint_position, 1000, NULL) == MR_ERROR_ACK1) {
        qDebug() << "STOPPING";
    }
    jointSetPosition(m_JointHandle, 0.0f, -1, NULL);

    while(fabs(pos -0.0f) > 0.01f) {
        jointPoll(m_JointHandle, &pos, &vel, &curr);
        qDebug("pos: %f\n", pos);
        Sleep(1);
    }
}

void FrictionTest::on_pB_Home_clicked()
{
    float pos = 0.0f, vel, curr;
    JOINT_HANDLE h;
    for (int jointid = 7; jointid >= 1; jointid--) {
        h = jointUp(jointid, MASTER(0));
        if (h) {
            if (jointSetMode(h, joint_position, 1000, NULL) == MR_ERROR_ACK1) {
                qDebug() << jointid << " CONNECTED";
            }
            jointSetScpInterval(h, 10, -1, NULL);

        } else {
            qDebug() << jointid << "Connect FAILED";
            continue;
        }

        jointSetMaxSpeed(h, 10., -1, NULL);
        jointSetMaxAcceleration(h, 50., -1, NULL);

        jointSetPosition(h, 0.0f, -1, NULL);
        do {
            jointPoll(h, &pos, &vel, &curr);
            qDebug("joint: %d, pos: %f\n", jointid, pos);
            Sleep(1);
        }while(fabs(pos -0.0f) > 0.01f);
        jointSetScpInterval(h, 0, -1, NULL);
        jointDown(h);
    }
}

void FrictionTest::on_pB_Go_clicked()
{
    float pos = 0.0f, vel, curr;
    float Target = ui->lE_TargetPos->text().toFloat();

    if (jointSetMode(m_JointHandle, joint_position, 1000, NULL) == MR_ERROR_ACK1) {
        qDebug() << ui->lE_Jointid->text().toInt() << " set position mode";
    }
    jointSetScpInterval(m_JointHandle, 10, -1, NULL);
    jointSetMaxSpeed(m_JointHandle, ui->lE_Speed->text().toFloat(), -1, NULL);
    jointSetMaxAcceleration(m_JointHandle, ui->lE_Acceleration->text().toFloat(), -1, NULL);
    jointSetPosition(m_JointHandle, Target, -1, NULL);
    ui->pB_Go->setText("JOGGING");
    ui->pB_Go->update();

    do {
        jointPoll(m_JointHandle, &pos, &vel, &curr);
        qDebug("pos: %f\n", pos);
        Sleep(1);
    }while(fabs(pos -Target) > 0.01f);
    jointSetScpInterval(m_JointHandle, 0, -1, NULL);
    ui->pB_Go->setText("Go");
    ui->pB_Go->update();
}

void FrictionTest::on_pushButton_clicked()
{
    float pos = 0.0f, vel, curr;
    JOINT_HANDLE h;
    jointDown(m_JointHandle);
    float Target = ui->lE_TargetPos->text().toFloat();
    for (int jointid = 7; jointid >= 1; jointid--) {
        h = jointUp(jointid, MASTER(0));
        if (h) {
            if (jointSetMode(h, joint_position, 1000, NULL) == MR_ERROR_ACK1) {
                qDebug() << jointid << " CONNECTED";
            }
            jointSetScpInterval(h, 10, -1, NULL);

        } else {
            qDebug() << jointid << "Connect FAILED";
            continue;
        }

        jointSetMaxSpeed(h, ui->lE_Speed->text().toFloat(), -1, NULL);
        jointSetMaxAcceleration(h, ui->lE_Acceleration->text().toFloat(), -1, NULL);

        jointSetPosition(h, Target, -1, NULL);
    }
    for (int jointid = 7; jointid >= 1; jointid--) {
        h = jointSelect(jointid);
        do {
            jointPoll(h, &pos, &vel, &curr);
            qDebug("joint: %d, pos: %f\n", jointid, pos);
            Sleep(1);
        }while(fabs(pos - Target) > 0.01f);
        jointSetScpInterval(h, 0, -1, NULL);
        jointDown(h);
    }

}
