#include "setandhealth.h"
#include "ui_setandhealth.h"
#include <QMessageBox>
#include <QDebug>

#define MONITOR_INTEVAL 500         // 监视器更新周期（ms）

SetAndHealth::SetAndHealth(QWidget *parent) :
    QWidget(parent),
    uiSetAndHealth(new Ui::SetAndHealth)
{
    uiSetAndHealth->setupUi(this);
    timer = NULL;
}

SetAndHealth::~SetAndHealth()
{
    delete uiSetAndHealth;
}

void SetAndHealth::SetAndHealthIint(int ID)
{
    if(!m_joint) {
        return ;
    }
    this->set();
    this->health();
}

void SetAndHealth::set()
{
    if(!m_joint) {
        return ;
    }
    uint16_t data16 = 0;
    jointGet(SYS_ENABLE_ON_POWER, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
    // 初始化API中的MCT里的ENABLE_ON_POWER
//    QString ENonPPushButtonOnStr = "color:green";
//    QString ENonPPushButtonOffStr = "color:red";
    if (data16 != 0) {
      uiSetAndHealth->ENonPPushButton->setStyleSheet("color:green");
    } else {
      uiSetAndHealth->ENonPPushButton->setStyleSheet("color:red");
    }
    // 读取ID 不能放在定时器里面,不然一直更新
//    jointGet(SYS_ID, 2, (Joint *)m_joint, (void *)&data16, 50, NULL);
//    uiSetAndHealth->IDLineEdit->setText(QString::number(data16));
    // 全局变量,不需要读取了
    uiSetAndHealth->IDLineEdit->setText(QString::number(sys_id));
}

void SetAndHealth::health()
{
    if(!m_joint) {
        return ;
    }
    if(!timer) {
        timer = new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(myTimerSlot()));
        timer->start(MONITOR_INTEVAL);
    }
    this->on_updateButton_clicked();
}

void SetAndHealth::myTimerSlot()
{
    if(!m_joint) {
        return ;
    }
    uint16_t udata16[15] = {0};
    jointGet(SYS_CURRENT_L, 4, (Joint *)m_joint, (void *)&udata16[1], 50, NULL);
    jointGet(SYS_SPEED_L, 4, (Joint *)m_joint, (void *)&udata16[7], 50, NULL);
    jointGet(SYS_POSITION_L, 4, (Joint *)m_joint, (void *)&udata16[9], 50, NULL);
    // 更新电流，并显示
    double temp =  udata16[1] + (udata16[2] * 65536);
    uiSetAndHealth->currentLineEdit->setText(QString::number(temp / 1000, 'f', 2) + "A");
    // 更新速度，并显示
    float speed = (float) ((udata16[7] + udata16[8] * 65536) * 60.0 / 65536.0 / sys_redu_ratio);
    uiSetAndHealth->speedLineEdit->setText(QString::number(speed, 'f', 2) + "rpm");
    // 更新位置，并显示
    uiSetAndHealth->pOS_LLineEdit->setText(QString::number((double)udata16[9] * 360.0 / 65536.0, 'f', 2) + "°");
    uiSetAndHealth->pOS_HLineEdit->setText(QString::number((short)udata16[10]));
}

void SetAndHealth::on_IDPushButton_clicked()
{
    // 不断电重启的话既不能检测到旧ID,也不能用新ID
    if(!m_joint) {
        return ;
    }
    int newID = uiSetAndHealth->IDLineEdit->text().toShort();
    if(newID > MAX_JOINTS - 1) {
        QMessageBox::warning(this,"Waring","ID too large");
    }else {
        // 设新的ID
        jointSet(SYS_ID, 2, (Joint *)m_joint, (void*)&newID, 100, NULL);
        // 之后需要重新更新ID在烧flash，这样才能设置成功
        QMessageBox::information(this, " 提示 ", "  请将模块重新上电并在软件中更新ID  ");
    }
}

void SetAndHealth::on_setZeroPushButton_clicked()
{
    if(!m_joint) {
        return ;
    }
    uint16_t workMode = 0;
    jointGet(TAG_WORK_MODE, 2, (Joint *)m_joint, (void *)&workMode, 50, NULL);
    if(workMode == 3) {
        emit ZeroPositionSeted();
        jointSetZero(m_joint,100,NULL);
        QMessageBox::information(this, tr(" NOTE "), tr("  succeed，please load flash  \n\n"), QMessageBox::Ok);
        emit ZeroPositionSeted();
    }
}

void SetAndHealth::on_ENonPPushButton_clicked()
{
    if(!m_joint) {
        return ;
    }
    uint16_t data_L = 0;
    jointGet(SYS_ENABLE_ON_POWER, 2, (Joint *)m_joint, (void *)&data_L, 50, NULL);
    int value = 0;
    if (data_L != 0) {
      value = 0;
    } else {
      value = 1;
    }
    // 向下位机请求数据
    jointSet(SYS_ENABLE_ON_POWER, 2, (Joint *)m_joint, (void *)&value, 50, NULL);
#ifdef LHDEBUG
    qDebug("SetAndHealth::on_ENonPPushButton_clicked(): value = %d", value);
#endif
    set();
}

void SetAndHealth::on_clearErrorButton_clicked()
{
    if(!m_joint) {
        return ;
    }
    jointSetClearError(m_joint, 100, NULL);
#ifdef LHDEBUG
    qDebug() << tr("SetAndHealth::on_clearErrorButton_clicked(): done.");
#endif
}

void SetAndHealth::on_updateButton_clicked()
{
    if(!m_joint) {
        return ;
    }
    uint16_t udata16[15] = {0};
    jointGet(SYS_VOLTAGE, 2, (Joint *)m_joint, (void *)&udata16[0], 50, NULL);
    jointGet(SYS_CURRENT_L, 4, (Joint *)m_joint, (void *)&udata16[1], 50, NULL);
    jointGet(SYS_TEMP, 2, (Joint *)m_joint, (void *)&udata16[3], 50, NULL);
    jointGet(ACC_X, 6, (Joint *)m_joint, (void *)&udata16[4], 50, NULL);
    jointGet(SYS_SPEED_L, 4, (Joint *)m_joint, (void *)&udata16[7], 50, NULL);
    jointGet(SYS_POSITION_L, 4, (Joint *)m_joint, (void *)&udata16[9], 50, NULL);
    jointGet(MOT_ST_DAT, 4, (Joint *)m_joint, (void *)&udata16[11], 50, NULL);
    jointGet(BAT_VOLT, 2, (Joint *)m_joint, (void *)&udata16[13], 50, NULL);
    jointGet(SYS_ERROR, 2, (Joint *)m_joint, (void *)&udata16[14], 50, NULL);
    // 更新电压，并显示
    uiSetAndHealth->voltageLineEdit->setText(QString::number((double)udata16[0] / 100, 'f', 2) + "V");
    // 更新电流，并显示
    double temp =  udata16[1] + (udata16[2] * 65536);
    uiSetAndHealth->currentLineEdit->setText(QString::number(temp / 1000, 'f', 2) + "A");
    // 更新温度，并显示
    uiSetAndHealth->tempLineEdit->setText(QString::number(static_cast<double>(udata16[3]) / 10, 'f', 1) + "°C");
    // 更新三轴加速度,并显示
    uiSetAndHealth->accXLineEdit->setText(QString::number((short)udata16[4]) + "mg");
    uiSetAndHealth->accYLineEdit->setText(QString::number((short)udata16[5]) + "mg");
    uiSetAndHealth->accZLineEdit->setText(QString::number((short)udata16[6]) + "mg");
    // 更新速度，并显示
    float speed = (float) ((udata16[7] + udata16[8] * 65536) * 60.0 / 65536.0 / sys_redu_ratio);
    uiSetAndHealth->speedLineEdit->setText(QString::number(speed, 'f', 2) + "rpm");
    // 更新位置，并显示
    uiSetAndHealth->pOS_LLineEdit->setText(QString::number((double)udata16[9] * 360.0 / 65536.0, 'f', 2) + "°");
    uiSetAndHealth->pOS_HLineEdit->setText(QString::number((short)udata16[10]));
    // 更新编码器，并显示
    uiSetAndHealth->s_TurnLineEdit->setText(QString::number((double)udata16[11] * 360.0/65536.0, 'f', 2) + "°");
    uiSetAndHealth->m_TurnLineEdit->setText(QString::number((short)udata16[12]));

    // 更新电池(编码器电池)电压，并显示
    if(udata16[13] == 0) {
        uiSetAndHealth->bATVoltLineEdit->setText("-");
    }else {
        uiSetAndHealth->bATVoltLineEdit->setText(QString::number((double)udata16[13] / 100, 'f', 2) + "V");
    }
    // 更新错误码，并显示
    // 按错误码显示文字
    uiSetAndHealth->errorTextEdit->clear();
    if (udata16[14] & ERROR_MASK_OVER_CURRENT) {
      uiSetAndHealth->errorTextEdit->append("过流，错误码0x0001\n");
    }
    if (udata16[14] & ERROR_MASK_OVER_VOLTAGE) {
      uiSetAndHealth->errorTextEdit->append("过压，错误码0x0002\n");
    }
    if (udata16[14] & ERROR_MASK_UNDER_VOLTAGE) {
      uiSetAndHealth->errorTextEdit->append("欠压，错误码0x0004\n");
    }
    if (udata16[14] & ERROR_MASK_OVER_TEMP) {
      uiSetAndHealth->errorTextEdit->append("过温,错误码0x0008\n");
    }
    if (udata16[14] & ERROR_MASK_BATTERY) {
      uiSetAndHealth->errorTextEdit->append("编码器电池错误,错误码0x0010\n");
    }
    if (udata16[14] & ERROR_MASK_ENCODER) {
      uiSetAndHealth->errorTextEdit->append("码盘错误,错误码0x0020\n");
    }
    if (udata16[14] & ERROR_MASK_POTEN) {
      uiSetAndHealth->errorTextEdit->append("电位器错误,错误码0x0040\n");
    }
    if (udata16[14] & ERROR_MASK_CURRENT_INIT) {
      uiSetAndHealth->errorTextEdit->append("电流检测错误,错误码0x0080\n");
    }
    if (udata16[14] & ERROR_MASK_FUSE) {
      uiSetAndHealth->errorTextEdit->append("保险丝断开错误,错误码0x0100\n");
    }
    if (udata16[14] == 0) {
      uiSetAndHealth->errorTextEdit->append("No Error\n");
    }
    this->myTimerSlot();
}
