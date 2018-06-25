#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),m_Connected(false),
    ui(new Ui::Widget), m_Dynamics(NULL)
{
    if (MR_ERROR_OK == startMaster("pcanusb1", MASTER(0))){
        qDebug() << "Master Started";
    } else {
        qDebug() << "Master Started ERROR";
    }

    ui->setupUi(this);

    QIntValidator* aIntValidator = new QIntValidator;
    aIntValidator->setRange(1, 50);
    ui->lE_Identity->setValidator(aIntValidator);

    QDoubleValidator* aDoubleValidator = new QDoubleValidator;

    ui->lE_Length->setValidator(aDoubleValidator);
    ui->lE_Weight->setValidator(aDoubleValidator);
    ui->lE_Kt->setValidator(aDoubleValidator);

    ui->lE_Length->setText(QString("%1").arg(185));
    ui->lE_Weight->setText(QString("%1").arg(536));
    ui->lE_Kt->setText(QString("%1").arg(4.5));

    ui->lE_Length->update();
    ui->lE_Weight->update();
    ui->lE_Kt->update();

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(1000);  // 1000ms

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pB_Connect_clicked()
{
    uint16_t jointid = ui->lE_Identity->text().toInt();
    uint16_t data;
    if (!m_Connected) {
        m_JointHandle = (Joint*)jointUp(jointid, MASTER(0));
        if (m_JointHandle) {
            if (jointSetMode(m_JointHandle, joint_current, 1000, NULL) == MR_ERROR_ACK1) {
                qDebug() << "CONNECTED";
            }
            jointSetScpInterval(m_JointHandle, 10, -1, NULL);
            jointSetScpMask(m_JointHandle, 0, -1, NULL);
            ui->pB_Connect->setText("connect");
            ui->pB_Connect->setStyleSheet("background-color: rgb(0, 255, 0);");
            ui->pB_Connect->update();
            m_Connected = true;
            m_Dynamics = new DyanamicsThread(this, m_JointHandle);
            m_Dynamics->setPayload(0.0f);
            m_Dynamics->start();

//            jointGet(TC_COUL_COEFF, 32, m_JointHandle, NULL, 15000, NULL);

            jointGet(TC_COUL_COEFF, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Coul_Coeff->setText(QString("%1").arg((float)data/1000.0f));

            jointGet(TC_FRICRISE_COEFF, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_CoulRise_Coeff->setText(QString("%1").arg((float)data/10000.0f));

            jointGet(TC_COUL_P, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Coul_P->setText(QString("%1").arg((float)data));

            jointGet(TC_COUL_N, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Coul_N->setText(QString("%1").arg((float)data));

            jointGet(TC_VISCOEFF_P, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Vis_P->setText(QString("%1").arg((float)data));

            jointGet(TC_VISCOEFF_N, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Vis_N->setText(QString("%1").arg((float)data));

            jointGet(TC_COUL_TEMP_P, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Coul_Temp_P->setText(QString("%1").arg((float)data));

            jointGet(TC_COUL_TEMP_N, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Coul_Temp_N->setText(QString("%1").arg((float)data));

            jointGet(TC_VISCOEFF_TEMP_P, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Vis_Temp_P->setText(QString("%1").arg((float)data));

            jointGet(TC_VISCOEFF_TEMP_N, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Vis_Temp_N->setText(QString("%1").arg((float)data));

            jointGet(TC_OSC_DEDUC, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_osc_amp->setText(QString("%1").arg((float)data/100.0f));

            jointGet(TC_OSC_FREQ, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_osc_freq->setText(QString("%1").arg((float)data/10.0f));

            jointGet(TC_ZEROVEL_THR, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_ZeroVel->setText(QString("%1").arg((float)data/100.0f));

            jointGet(TC_CRITACC_THR, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_CrtiAcc->setText(QString("%1").arg((float)data/100.0f));

            jointGet(TC_STARTUP_TW, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Startup_TW->setText(QString("%1").arg((float)data));

            jointGet(TC_STOP_TW, 2, m_JointHandle, &data, 5000, NULL);
            ui->lE_Stop_TW->setText(QString("%1").arg((float)data));
        } else {
            qDebug() << jointid <<"Connect FAILED";
            m_Connected = false;
        }
    } else {
        jointDown((JOINT_HANDLE*)&m_JointHandle);
        qDebug() << "DISCONNECTED";
        ui->pB_Connect->setText("连接");
        ui->pB_Connect->setStyleSheet("background-color: rgb(240, 240, 240);");
        ui->pB_Connect->update();
        m_Connected = false;
        m_Dynamics->stop();
        m_Dynamics->terminate();
        delete m_Dynamics;
        m_Dynamics = NULL;
    }
}

void Widget::update()
{
    float position, current;
    if (m_Dynamics)
        m_Dynamics->getJointState(position, current);
    ui->lB_Current->setText(QString("%1").arg(current));
    ui->lB_Position->setText(QString("%1").arg(position));

    ui->lB_Current->update();
    ui->lB_Position->update();

}


void Widget::on_lE_Weight_editingFinished()
{
    float weight = ui->lE_Weight->text().toFloat()/1000;
    float length = ui->lE_Length->text().toFloat()/1000;
    float kt = ui->lE_Kt->text().toFloat();

    if (m_Dynamics) {
        m_Dynamics->setPayload(weight*9.81*length/kt);  // A
        qDebug() << "Set Payload" << weight*9.81*length/kt;
    }
}

void Widget::on_lE_Length_editingFinished()
{
    float weight = ui->lE_Weight->text().toFloat()/1000;
    float length = ui->lE_Length->text().toFloat()/1000;
    float kt = ui->lE_Kt->text().toFloat();

    if (m_Dynamics)
    m_Dynamics->setPayload(weight*9.81*length/kt);  // A
}

void Widget::on_lE_Kt_editingFinished()
{
    float weight = ui->lE_Weight->text().toFloat()/1000;
    float length = ui->lE_Length->text().toFloat()/1000;
    float kt = ui->lE_Kt->text().toFloat();

    if (m_Dynamics)
    m_Dynamics->setPayload(weight*9.81*length/kt);  // A
}

void Widget::on_lE_Coul_P_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Coul_P->text().toFloat());

    jointSet(TC_COUL_P, 2, m_JointHandle, &data, -1, NULL);

}

void Widget::on_lE_Coul_N_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Coul_N->text().toFloat());

    jointSet(TC_COUL_N, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Vis_P_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Vis_P->text().toFloat());

    jointSet(TC_VISCOEFF_P, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Vis_N_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Vis_N->text().toFloat());

    jointSet(TC_VISCOEFF_N, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Coul_Coeff_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Coul_Coeff->text().toFloat()*1000.0f);

    jointSet(TC_COUL_COEFF, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_CoulRise_Coeff_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_CoulRise_Coeff->text().toFloat()*10000.0f);

    jointSet(TC_FRICRISE_COEFF, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_osc_freq_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_osc_freq->text().toFloat()*10.0f);

    jointSet(TC_OSC_FREQ, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_osc_amp_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_osc_amp->text().toFloat()*100.0f);

    jointSet(TC_OSC_DEDUC, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_ZeroVel_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_ZeroVel->text().toFloat()*100.0f);

    jointSet(TC_ZEROVEL_THR, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_CrtiAcc_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_CrtiAcc->text().toFloat()*100.0f);

    jointSet(TC_CRITACC_THR, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Startup_TW_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Startup_TW->text().toFloat());

    jointSet(TC_STARTUP_TW, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Stop_TW_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Stop_TW->text().toFloat());

    jointSet(TC_STOP_TW, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_Widget_destroyed()
{
    qDebug() << "Closing window";
    m_Dynamics->stop();
    m_Dynamics->wait();
    m_Dynamics->exit(0);

    delete m_Dynamics;
    jointDown((JOINT_HANDLE*)&m_JointHandle);
    stopMaster(0);
    qDebug() << "window closed";
}

void Widget::on_pB_Save2Flash_clicked()
{
    jointSetSave2Flash(m_JointHandle, 1000, NULL);
}

void Widget::on_lE_Coul_Temp_P_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Coul_Temp_P->text().toFloat());

    jointSet(TC_COUL_TEMP_P, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Coul_Temp_N_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Coul_Temp_N->text().toFloat());

    jointSet(TC_COUL_TEMP_N, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Vis_Temp_P_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Vis_Temp_P->text().toFloat());

    jointSet(TC_VISCOEFF_TEMP_P, 2, m_JointHandle, &data, -1, NULL);
}

void Widget::on_lE_Vis_Temp_N_editingFinished()
{
    uint16_t data = (uint16_t)(ui->lE_Vis_Temp_N->text().toFloat());

    jointSet(TC_VISCOEFF_TEMP_N, 2, m_JointHandle, &data, -1, NULL);
}
