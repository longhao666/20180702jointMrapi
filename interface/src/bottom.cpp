#include "bottom.h"
#include "ui_bottom.h"
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QTimer>


using std::vector;
using std::sort;

#define LHDEBUG 0
#define BOTTOM_UPDATE_INTEVAL 3000

uint16_t sys_redu_ratio = 1000;
uint16_t tag_work_mode = 1000;
uint16_t sys_id = 1000;

vector<uint32_t> vectID;


Bottom::Bottom(QWidget *parent) :
    QWidget(parent),
    uiBottom(new Ui::Bottom)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiBottom->setupUi(this);
    timerBottom = NULL;
    isCANInitialSucceed = false;
#if 0
    uiBottom->enableDriverPushButton_2->setEnabled(false);
    uiBottom->ifErrorPushButton->setEnabled(false);
    uiBottom->connectedPushButton->setEnabled(false);
    uiBottom->workModePushButton->setEnabled(false);
#endif
}

Bottom::~Bottom()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiBottom;
}

void Bottom::waitingForWidgetReady()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    this->on_btnUpdateID_clicked();
}

void Bottom::slotFreeUpMemory()
{
    m_joint = NULL;
    for(vector<uint32_t>::iterator iter=vectID.begin();
        iter != vectID.end();
        ++iter) {
        m_joint_copy = jointSelect(*iter);
        qDebug() << "m_joint_copy = " << m_joint_copy;
        jointDown(&m_joint_copy);
    }
}

void Bottom::updateEnableDriver()
{
    uint16_t data16 = 0;
    jointGet(SYS_ENABLE_DRIVER, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    if(data16) {
        uiBottom->enableDriverPushButton_2->setStyleSheet("background-color:green");
        uiBottom->enableDriverPushButton->setText("Enabled");
    }else {
        uiBottom->enableDriverPushButton_2->setStyleSheet("");
        uiBottom->enableDriverPushButton->setText("Disabled");
    }
}

void Bottom::updateWorkModePushButton()
{
    uint16_t workMode = 0;
    uiBottom->workModePushButton->setStyleSheet("background-color:green");
    jointGetMode(m_joint, &workMode, 50, NULL);
    switch (workMode) {
    case 0:
        uiBottom->workModePushButton->setText("OPEN");
        break;
    case 1:
        uiBottom->workModePushButton->setText("CURRENT");
        break;
    case 2:
        uiBottom->workModePushButton->setText("SPEED");
        break;
    case 3:
        uiBottom->workModePushButton->setText("POSITION");
        break;
    default:
        break;
    }
}

void Bottom::updateConnected()
{
    uiBottom->connectedPushButton->setStyleSheet("background-color:green");
}

void Bottom::updateIfError()
{
    uint16_t data16 = 0;
    jointGet(SYS_ERROR, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    if(data16 != 0) {
        uiBottom->ifErrorPushButton->setStyleSheet("");
    }else {
        uiBottom->ifErrorPushButton->setStyleSheet("background-color:green");
    }
}

void Bottom::updatecmbID()
{
    uiBottom->cmbID->clear();
    vectID.clear();
    JOINT_HANDLE tempj = NULL;
    uint16_t ID = 0;
    for(int i=1;i<2;i++) {
        tempj = jointUp(i, 0);
        if(tempj) {
            int re = jointGet(SYS_ID, 2, (Joint *)tempj, (void *)&ID, 100, NULL);
            vectID.push_back(ID);
            qDebug() << "发现这个ID:" << ID << re;
        }
    }
    if(vectID.empty()) {
//        qDebug() << vectID.empty();
        this->isCANInitialSucceed = false;
        QMessageBox::warning(this,"WARNING","Module not detected", QMessageBox::Ok);
        return ;
    }
    sort(vectID.begin(), vectID.end());
#if 0
    qDebug() << "vectID.back()" << vectID.back();
#endif
    m_joint = jointSelect(vectID.back());
    for(vector<uint32_t>::iterator iter = vectID.begin();
        iter != vectID.end();
        ++iter) {
        uiBottom->cmbID->addItem(QString::number(*iter, 10));
    }
    uiBottom->cmbID->setCurrentIndex((int)(vectID.size() - 1));
}

void Bottom::on_btnUpdateID_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!isCANInitialSucceed) {
        isCANInitialSucceed = true;
        char str[] = "pcanusb1";
        qDebug("===============");
        int re = startMaster(str, 0);
        qDebug() << re << "startMaster(0)";
        this->updatecmbID();
    }
    else {
        QMessageBox::warning(this,tr("提示"),
                             tr("  更新失败\n 如果你确定想更新ID  \n 请按quit后再进行更新ID  "),
                             QMessageBox::Ok);
    }
}

void Bottom::on_enableDriverPushButton_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!m_joint) {
        return ;
    }
    uint16_t data16 = 0;
    jointGet(SYS_ENABLE_DRIVER, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    bool isEbable = !data16;
    uint16_t value = (int)isEbable;
    qDebug() << data16 << isEbable;
    jointSet(SYS_ENABLE_DRIVER, 2, (Joint *)m_joint, (void *)&value, 50, NULL);
    updateEnableDriver();
}

void Bottom::on_btnFlash_clicked()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!m_joint) {
        return ;
    }
    jointSetSave2Flash(m_joint, 50, NULL);
    QMessageBox::information(this, tr("information"), tr("    Succeed     "), QMessageBox::Ok);
}

void Bottom::on_btnLoad_clicked()
{
#if 0
    QMessageBox::information(this, tr("information"), tr("该功能还没实现！"), QMessageBox::Ok);
#endif
    emit signalBtnLoadClicked();
}

void Bottom::on_btnSave_clicked()
{
#if 0
    QMessageBox::information(this, tr("information"), tr("该功能还没实现！"), QMessageBox::Ok);
#endif
    if(!m_joint) {
        return ;
    }
    emit signalBtnSaveClicked();
}

void Bottom::on_cmbID_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!m_joint) {
        return ;
    }
    sys_id = uiBottom->cmbID->currentText().toInt();
    m_joint = jointSelect(sys_id);
#if 1
    qDebug() << "sys_id  = " << sys_id << "; index = " << index << "; on_cmbID_currentIndexChanged";
    qDebug() << "m_joint = " << m_joint;
#endif
    emit cmbIDChanged(sys_id);
    emit signalRecoverBotton();
    if(!timerBottom) {
        timerBottom = new QTimer(this);
        connect(timerBottom, SIGNAL(timeout()), this, SLOT(slotTimerBottomDone()));
        timerBottom->start(BOTTOM_UPDATE_INTEVAL);
    }
    if(!timerBottom->isActive()) {
        timerBottom->start();
    }
    slotTimerBottomDone();
    uint16_t data16 = 0;
    jointGet(SYS_MODEL_TYPE, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    QString str;
    switch(data16) {
    case MODEL_TYPE_M14:
        str = "M14";
        break;
    case MODEL_TYPE_M17:
        str = "M17";
        break;
    case MODEL_TYPE_M17V2:
        str = "M17E";
        break;
    case MODEL_TYPE_M20:
        str = "M20";
        break;
    case MODEL_TYPE_M20V2:
        str = "M20E";
        break;
    default :
        break;
    }
    uiBottom->typeLabel->setText(str);
    jointGet(SYS_REDU_RATIO, 2, (Joint *)m_joint, (void *)&sys_redu_ratio, 50, NULL);
    uiBottom->ratioLabel->setText(QString::number(sys_redu_ratio, 10));
    jointGet(SYS_FW_VERSION, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    uiBottom->firmLabel->setText(QString::number(data16, 10));
}

void Bottom::slotTimerBottomDone()
{
    if(!m_joint) {
        return ;
    }
    updateEnableDriver();
    updateWorkModePushButton();
    updateConnected();
    updateIfError();
}

void Bottom::on_btnQuit_clicked()
{
    if(!m_joint) {
        return ;
    }
    int re = QMessageBox::information(this, tr(" 提示 "), tr(" 确定要退出吗? "), QMessageBox::Yes, QMessageBox::No);
    if(re == QMessageBox::Yes) {
        qDebug() << "re " << re << "yes";
        isCANInitialSucceed = false;
        m_joint = NULL;
        for(vector<uint32_t>::iterator iter=vectID.begin();
            iter != vectID.end();
            ++iter) {
            m_joint_copy = jointSelect(*iter);
            qDebug() << "m_joint_copy =" << m_joint_copy;
            jointDown(&m_joint_copy);
        }
        emit signalRecoverBotton();
        uiBottom->enableDriverPushButton_2->setStyleSheet("");
        uiBottom->workModePushButton->setStyleSheet("");
        uiBottom->connectedPushButton->setStyleSheet("");
        uiBottom->ifErrorPushButton->setStyleSheet("");
        timerBottom->stop();
    }else if(re == QMessageBox::No){
        qDebug() << "re " << re << "no";
    }else {
        qDebug() << "nothingNess";
    }
}
