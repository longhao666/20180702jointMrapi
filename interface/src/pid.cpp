#include "pid.h"
#include "ui_pid.h"
#include <QDebug>


#define LHDEBUG 0

Pid::Pid(QWidget *parent) :
    QWidget(parent),
    uiPid(new Ui::Pid)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiPid->setupUi(this);
    joint = NULL;
}

Pid::~Pid()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiPid;
}

void Pid::pidInit(int ID)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;

#endif
#if 0
    qDebug() << "ID = " << ID;
#endif
    this->joint = jointSelect(ID);
    if(!joint) {
        return ;
    }
    on_adjustGroupComboBox_currentIndexChanged(uiPid->adjustGroupComboBox->currentIndex());
    showMaxLimit();
}

void Pid::showSEVPID()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    // 位置环P
    jointGet(S_POSITION_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_PSpinBox->setValue(data16);
    // 位置环I
    jointGet(S_POSITION_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_ISpinBox->setValue(data16);
    // 位置环D
    jointGet(S_POSITION_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSpinBox->setValue(data16);
    // 位置死区
    jointGet(S_POSITION_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSSpinBox->setValue(data16);
    // 速度环P;
    jointGet(S_SPEED_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_PSpinBox->setValue(data16);
    // 速度环I
    jointGet(S_SPEED_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_ISpinBox->setValue(data16);
    // 速度环D
    jointGet(S_SPEED_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSpinBox->setValue(data16);
    // 速度死区
    jointGet(S_SPEED_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSSpinBox->setValue(data16);
    // 电流环P
    jointGet(S_CURRENT_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_PSpinBox->setValue(data16);
    // 电流环I
    jointGet(S_CURRENT_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_ISpinBox->setValue(data16);
    // 电流环D
    jointGet(S_CURRENT_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_DSpinBox->setValue(data16);
}

void Pid::showMPID()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    // 位置环P
    jointGet(M_POSITION_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_PSpinBox->setValue(data16);
    // 位置环I
    jointGet(M_POSITION_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_ISpinBox->setValue(data16);
    // 位置环D
    jointGet(M_POSITION_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSpinBox->setValue(data16);
    // 位置死区
    jointGet(M_POSITION_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSSpinBox->setValue(data16);
    // 速度环P;
    jointGet(M_SPEED_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_PSpinBox->setValue(data16);
    // 速度环I
    jointGet(M_SPEED_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_ISpinBox->setValue(data16);
    // 速度环D
    jointGet(M_SPEED_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSpinBox->setValue(data16);
    // 速度死区
    jointGet(M_SPEED_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSSpinBox->setValue(data16);
    // 电流环P
    jointGet(M_CURRENT_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_PSpinBox->setValue(data16);
    // 电流环I
    jointGet(M_CURRENT_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_ISpinBox->setValue(data16);
    // 电流环D
    jointGet(M_CURRENT_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_DSpinBox->setValue(data16);
}

void Pid::showLPID()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    uint16_t data16 = 0;
    // 位置环P
    jointGet(L_POSITION_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_PSpinBox->setValue(data16);
    // 位置环I
    jointGet(L_POSITION_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_ISpinBox->setValue(data16);
    // 位置环D
    jointGet(L_POSITION_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSpinBox->setValue(data16);
    // 位置死区
    jointGet(L_POSITION_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->POS_DSSpinBox->setValue(data16);
    // 速度环P;
    jointGet(L_SPEED_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_PSpinBox->setValue(data16);
    // 速度环I
    jointGet(L_SPEED_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_ISpinBox->setValue(data16);
    // 速度环D
    jointGet(L_SPEED_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSpinBox->setValue(data16);
    // 速度死区
    jointGet(L_SPEED_DS, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->SPD_DSSpinBox->setValue(data16);
    // 电流环P
    jointGet(L_CURRENT_P, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_PSpinBox->setValue(data16);
    // 电流环I
    jointGet(L_CURRENT_I, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_ISpinBox->setValue(data16);
    // 电流环D
    jointGet(L_CURRENT_D, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->CUR_DSpinBox->setValue(data16);
}

void Pid::showMaxLimit()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uint16_t data16 = 0;
    // 最大加速度
    jointGet(LIT_MAX_ACC, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->maxAccLineEdit->setText(QString::number(data16, 10));
    // 最大速度
    jointGet(LIT_MAX_SPEED, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->maxSpdLineEdit->setText(QString::number(data16, 10));

    jointGet(LIT_MAX_CURRENT, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->maxCurLineEdit->setText(QString::number(data16, 10));

    jointGet(LIT_MIN_POSITION_L, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->minPosLLineEdit->setText(QString::number(data16, 10));

    jointGet(LIT_MIN_POSITION_H, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->minPosHLineEdit->setText(QString::number(data16, 10));

    jointGet(LIT_MAX_POSITION_L, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->maxPosLLineEdit->setText(QString::number(data16, 10));

    jointGet(LIT_MAX_POSITION_H, 2, (Joint *)joint, (void *)&data16, 50, NULL);
    uiPid->maxPosHLineEdit->setText(QString::number(data16, 10));
}

void Pid::on_POS_PSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->POS_PSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->POS_PSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_POSITION_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_POSITION_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_POSITION_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_POS_ISpinBox_editingFinished()
{
    if(!joint) {
        uiPid->POS_ISpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->POS_ISpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_POSITION_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_POSITION_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_POSITION_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_POS_DSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->POS_DSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->POS_DSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_POSITION_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_POSITION_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_POSITION_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_POS_DSSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->POS_DSSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->POS_DSSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_POSITION_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_POSITION_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_POSITION_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_SPD_PSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_PSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->SPD_PSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_SPEED_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_SPEED_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_SPEED_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_SPD_ISpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->SPD_ISpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->SPD_ISpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_SPEED_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_SPEED_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_SPEED_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_SPD_DSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_DSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->SPD_DSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_SPEED_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_SPEED_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_SPEED_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_SPD_DSSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->SPD_DSSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->SPD_DSSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_SPEED_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_SPEED_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_SPEED_DS, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_CUR_PSpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->CUR_PSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->CUR_PSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_CURRENT_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_CURRENT_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_CURRENT_P, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_CUR_ISpinBox_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->CUR_ISpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->CUR_ISpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_CURRENT_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_CURRENT_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_CURRENT_I, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_CUR_DSpinBox_editingFinished()
{
    if(!joint) {
        uiPid->CUR_DSpinBox->setValue(0);
        return ;
    }
    uint16_t value = uiPid->CUR_DSpinBox->text().toShort();
    int index = uiPid->adjustGroupComboBox->currentIndex();
    switch(index) {
    case 0:
        jointSet(S_CURRENT_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 1:
        jointSet(M_CURRENT_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    case 2:
        jointSet(L_CURRENT_D, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    default :
        break;
    }
}

void Pid::on_adjustGroupComboBox_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    switch(index) {
        case 0:
            showSEVPID();
            break;
        case 1:
            showMPID();
            break;
        case 2:
            showLPID();
            break;
        default :
             break;
    }
}

void Pid::on_maxAccLineEdit_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        uiPid->maxAccLineEdit->setText("");
        return ;
    }
    QString str = uiPid->maxAccLineEdit->text();
    jointSetMaxAcceleration(joint, str.toShort(), 100, NULL);
}

void Pid::on_minPosLLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->minPosLLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->minPosLLineEdit->text().toShort();
    jointSet(LIT_MIN_POSITION_L, 2, (Joint *)joint, (void *)&value, 50, NULL);
}

void Pid::on_minPosHLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->minPosHLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->minPosHLineEdit->text().toShort();
    jointSet(LIT_MIN_POSITION_H, 2, (Joint *)joint, (void *)&value, 50, NULL);
}

void Pid::on_maxPosLLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxPosLLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->maxPosLLineEdit->text().toShort();
    jointSet(LIT_MAX_POSITION_L, 2, (Joint *)joint, (void *)&value, 50, NULL);
}

void Pid::on_maxPosHLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxPosHLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->maxPosHLineEdit->text().toShort();
    jointSet(LIT_MAX_POSITION_H, 2, (Joint *)joint, (void *)&value, 50, NULL);
}

void Pid::on_maxSpdLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxSpdLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->maxSpdLineEdit->text().toShort();
    jointSet(LIT_MAX_SPEED, 2, (Joint *)joint, (void *)&value, 50, NULL);
}

/**
 * @brief Pid::on_maxCurLineEdit_editingFinished
 */
void Pid::on_maxCurLineEdit_editingFinished()
{
    if(!joint) {
        uiPid->maxCurLineEdit->setText("");
        return ;
    }
    uint16_t value = uiPid->maxCurLineEdit->text().toShort();
    // 设置最大的电流
    jointSet(LIT_MAX_CURRENT, 2, (Joint *)joint, (void *)&value, 50, NULL);
}
