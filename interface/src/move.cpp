#include "move.h"
#include "ui_move.h"
#include <QDebug>
#include <QTimer>


#define LHDEBUG 0

#define PI 3.1415926
#define MODE_MANUAL     0           // 手动控制
#define MODE_SINE       1           // 自动控制之正弦波
#define MODE_SQUARE     2           // 自动控制之方波
#define MODE_TRIANGLE   3           // 自动控制之三角波
#define MOTION_CONTROL_INTEVAL 30   // 运动控制周期（ms）



Move::Move(QWidget *parent) :
    QWidget(parent),
    uiMove(new Ui::Move)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    uiMove->setupUi(this);
    joint = NULL;
    timerMove = NULL;
    bias = 0.0;
    amplitude = 0.0;
    frequency = 0.0;
}

Move::~Move()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    delete uiMove;
}

void Move::moveInit(int ID)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    // 定义运动控制为关
    enableRun = false;
    uiMove->confirmButton->setText("Click to run"); // 此外还有一处设置了setTex
    uiMove->stopButton->setStyleSheet("color:red");
    if(!timerMove) {
        timerMove = new QTimer;
        connect(timerMove, SIGNAL(timeout()), this, SLOT(slotTimeMoveDone()));
        //        timerMove->start(MOTION_CONTROL_INTEVAL); // 暂时不启动
    }
#if 0
    qDebug() << "ID = " << ID;
#endif
    joint = jointSelect(ID);
    if(!joint) {
        qDebug("===================empty=======================");
        return ;
    }
    uint16_t work;
//    jointGetMode(joint, &work, 50, NULL);
    jointGet(TAG_WORK_MODE, 4, (Joint *)joint, (void *)&work, 50, NULL);
    uiMove->cmbWorkMode->setCurrentIndex(work);
    // 防止没有调用on_cmbWorkMode_currentIndexChanged()，强制运行下列2个函数
    // 工作模式更新bias
    workModeUpdatetxtBias();
    // bias更新滑块
    txtBiasChangeManualSlider();

}

void Move::ClickStopButton()
{
#if 0
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!enableRun) {
        enableRun = true;
        on_stopButton_clicked();
    }else {
        on_stopButton_clicked();
    }
}

void Move::txtBiasChangeManualSlider()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    int workMode = uiMove->cmbWorkMode->currentIndex();
    double bias = uiMove->txtBias->text().toDouble();
    float min = uiMove->manualMin->text().toFloat();
    float max = uiMove->manualMax->text().toFloat();
    if(MODE_POSITION == workMode) { // 如果是位置控制模式，滑块极值相应改变
        if(uiMove->manualSlider->hasFocus() || uiMove->manualMax->hasFocus() || uiMove->manualMin->hasFocus()) {
            ;
        } else {
            max = bias + 90;
            min = bias - 90;
            uiMove->manualMax->setText(QString::number(max, 'f' , 2));
            uiMove->manualMin->setText(QString::number(min, 'f' , 2));
        }
    }
    float f = 100 * (bias - min) / (max - min);
    int v = (int)f;
    uiMove->manualSlider->setValue(v);
}

void Move::workModeUpdatetxtBias()
{
    if(!joint) {
        return ;
    }
    int32_t data32 = 0;
    int workMode = uiMove->cmbWorkMode->currentIndex();
    switch(workMode) { // 工作模式修改后，修改txtBias
    case MODE_OPEN:
        uiMove->txtBias->setValue(0); // 由当前目标PWM更新手动控制中的偏移量
        uiMove->manualMax->setText("30");
        uiMove->manualMin->setText("-30");
        break;
    case MODE_CURRENT:
        uiMove->txtBias->setValue(0); // 和上面注释的那句效果一样，不如就改成0
        uiMove->manualMax->setText("500");
        uiMove->manualMin->setText("-500");
        break;
    case MODE_SPEED: {
        uiMove->txtBias->setValue(0); // 由当前实际电流更新手动控制中的偏移量
        uiMove->manualMax->setText("20");
        uiMove->manualMin->setText("-20");
        break;
    }
    case MODE_POSITION: {
        jointGet(SYS_POSITION_L, 4, (Joint *)joint, (void *)&data32, 50, NULL);
        float tempf = (float)(data32);
        tempf = tempf * 360.0/65536.0;
        uiMove->txtBias->setValue(tempf); // 由当前实际位置更新手动控制中的偏移量
        uiMove->manualMax->setText("90");
        uiMove->manualMin->setText("-90");
        break;
    }
    default:
        break;
    }
    on_txtBias_editingFinished();
}

void Move::setMoveValue(int value)
{
    int workMode = uiMove->cmbWorkMode->currentIndex();
    switch(workMode) // 不同控制模式，控制指令不同
    {
    case MODE_OPEN: {
        jointSet(TAG_OPEN_PWM, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_CURRENT: {
        jointSet(TAG_CURRENT_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_SPEED: {
        static int count = 0;
        count++;
        uint16_t udata16 = 0;
#if 0
        qDebug() << "count = " << count << "udata16 = " << udata16 << "value = " << value;
#endif
        jointGet(SYS_REDU_RATIO, 2, (Joint *)joint, (void *)&udata16, 50, NULL);
        value *= udata16;
        value = value * 65536.0/60.0;
#if 0
        qDebug() << "count = " << count << "udata16 = " << udata16 << "value = " << value;
#endif
        jointSet(TAG_SPEED_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_POSITION: {
        value = value * 65536/360;
        jointSet(TAG_POSITION_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    default: break;
    }
}

/**
 * @brief Move::on_txtBias_editingFinished
 */
void Move::on_txtBias_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif 
    if(!joint) {
        qDebug("===================empty=======================");
        return ;
    }
    int workMode = uiMove->cmbWorkMode->currentIndex();
    if(MODE_POSITION != workMode) { // 若是位置控制，则不限制bias
        float max = uiMove->manualMax->text().toFloat();
        float min = uiMove->manualMin->text().toFloat();
        if(uiMove->txtBias->text().toInt() > max) {
            uiMove->txtBias->setValue(max);
        } else if (uiMove->txtBias->text().toInt() < min) {
            uiMove->txtBias->setValue(min);
        }
    }
    bias = uiMove->txtBias->text().toDouble();
    txtBiasChangeManualSlider();
}

/**
 * @brief Move::on_cmbWorkMode_currentIndexChanged
 * @param index
 * 选择工作模式
 */
void Move::on_cmbWorkMode_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    if(!joint) {
        return ;
    }
    // 更改工作模式
    jointSetMode(joint, index, 50, NULL);
    // 工作模式更新bias
    workModeUpdatetxtBias();
    // bias更新滑块
    txtBiasChangeManualSlider();
}

/**
 * @brief Move::on_waveModeCombo_currentIndexChanged
 * @param index
 * 选择运动方式
 */
void Move::on_waveModeCombo_currentIndexChanged(int index)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    switch(index) {
    case MODE_MANUAL:
        uiMove->frequencyLineEdit->setEnabled(false);
        uiMove->frequencyLineEdit->setText("");
        uiMove->AmplitudeLineEdit->setEnabled(false);
        uiMove->AmplitudeLineEdit->setText("");
        break;
    case MODE_SINE:
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        break;
    case MODE_SQUARE:
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        break;
    case MODE_TRIANGLE:
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
        break;
    default :
        break;
    }
}

void Move::on_frequencyLineEdit_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    frequency = uiMove->frequencyLineEdit->text().toDouble();
}

void Move::on_AmplitudeLineEdit_editingFinished()
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
    amplitude = uiMove->AmplitudeLineEdit->text().toDouble();
}

void Move::on_manualSlider_valueChanged(int value)
{
#if LHDEBUG
    qDebug() <<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;
#endif
#if 0
    qDebug() << uiMove->manualSlider->hasFocus();
#endif
    float min = uiMove->manualMin->text().toFloat();
    float max = uiMove->manualMax->text().toFloat();
    if(uiMove->manualSlider->hasFocus()) {
        double bias = min + (max - min) * value / 100;
        uiMove->txtBias->setValue(bias);
        on_txtBias_editingFinished();
    }
}

void Move::slotTimeMoveDone()
{
    if(!joint) {
#if 0
        qDebug() <<__FILE__<<__LINE__<<__func__;
        qDebug("===================empty===================");
#endif;
        return ;
    }
    if(enableRun) {
        static unsigned int s_iCount = 0;
        static unsigned int s_iCountforwave = 0;
        s_iCount++;
        //根据所选波形进入相应控制步骤
        switch (uiMove->waveModeCombo->currentIndex())
        {
        case MODE_MANUAL: {
            s_iCount = 0;
            //                qDebug("===========case MODE_MANUAL:========empty===================");
            //                qDebug() << bias;
            setMoveValue(bias);
            break;
        }
        case MODE_SQUARE: { // 方波时根据选定频率发送，经过半个周期变换一次方向，所以是乘500
            double time = s_iCount * MOTION_CONTROL_INTEVAL;
            if (time >= 500.0 / frequency) {
                static bool s_bHigh = false;
                s_iCount = 0;
                s_bHigh = !s_bHigh;
                setMoveValue((int)(amplitude * (s_bHigh ? 1 : -1) + bias));
            }
            break;
        }
        case MODE_TRIANGLE: {//三角波时 TriangleInterval 个 Interval 发送一次
            const unsigned int TriangleInterval = 1;
            if(s_iCount >= TriangleInterval) {
                s_iCount = 0;
                //得到三角波的周期（s）
                double T = 1.0 / frequency;
                //校准指令发送的间隔时间
                s_iCountforwave++;
                //获得发送指令时真正的时间（ms）
                double time = s_iCountforwave * TriangleInterval * MOTION_CONTROL_INTEVAL;
                //若当前时间超过一个周期，校准时间使得时间回到周期开始
                if(time / 1000 >= T) {
                    s_iCountforwave = 0;
                }
                //由当前时间得到当前控制值
                double tempf = time / 1000 * amplitude / T;
                //发送控制值
                setMoveValue(tempf + bias);
            }
            break;
        }
        case MODE_SINE: { // 正弦波时SineInterval 个Interval 发送一次
            const unsigned int SineInterval = 1;
            if(s_iCount >= SineInterval) {
                s_iCount = 0;
                //得到正弦波的周期（s）
                double T = 1.0 / frequency;
                //校准指令发送的间隔时间
                s_iCountforwave++;
                //获得发送指令时真正的时间（ms）
                double time = s_iCountforwave * SineInterval * MOTION_CONTROL_INTEVAL;
                //若当前时间超过一个周期，校准时间使得时间回到周期开始
                if(time / 1000 >= T) {
                    s_iCountforwave = 0;
                }
                //由当前时间得到当前控制值
                double tempf = sin(time / 1000 * frequency * 2 * PI) * amplitude;
                //发送控制值
                setMoveValue((short)(tempf + bias));
            }
            break;
        }
        }
    }
}

void Move::on_confirmButton_clicked()
{
    if(!joint) {
        return ;
    }
    if(!enableRun) {
        enableRun = true;
        uiMove->confirmButton->setText("Running");
        uiMove->confirmButton->setStyleSheet("QPushButton{color:red}");
        uiMove->stopButton->setStyleSheet("");
        timerMove->start(MOTION_CONTROL_INTEVAL);
    }else {
        return ;
    }
}

void Move::on_manualMin_editingFinished()
{
    txtBiasChangeManualSlider();
}

void Move::on_manualMax_editingFinished()
{
    float max = uiMove->manualMax->text().toFloat();
    QString s = QString("%1").arg(max * -1);
    uiMove->manualMin->setText(s);
    txtBiasChangeManualSlider();
}

/**
 * @brief Move::on_stopButton_clicked
 * 停止运动,关闭定时器
 */
void Move::on_stopButton_clicked()
{
    if(!joint) {
        return ;
    }
    if(enableRun) {
        enableRun = false;
        uiMove->stopButton->setStyleSheet("QPushButton{color:red;}");
        uiMove->confirmButton->setText("Click to run");
        uiMove->confirmButton->setStyleSheet("");
        timerMove->stop();
    }else {
        return ;
    }
    const int workMode = uiMove->cmbWorkMode->currentIndex();
    switch(workMode) // Different WorkMode Different Stop way
    {
    case MODE_OPEN: {
//        jointSetTAG_OPEN_PWM(joint, 0, 50, NULL);
        int value = 0;
        jointSet(TAG_OPEN_PWM, 2, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_CURRENT: {
//        jointSetTAG_CURRENT_L(joint, 0, 50, NULL);
        int value = 0;
        jointSet(TAG_CURRENT_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_SPEED: {
        // 猜测:按暂停后速度会马上变为0,应该是这里设置为0后,模块里面反应后减速一直到模块停下
//        jointSetTAG_SPEED_L(joint, 0, 50, NULL);
        int value = 0;
        jointSet(TAG_SPEED_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    case MODE_POSITION: {
        int value = 0;
        jointGet(SYS_POSITION_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        jointSet(TAG_POSITION_L, 4, (Joint *)joint, (void *)&value, 50, NULL);
        break;
    }
    default:
        break;
    }
    // In fact, Stop means setting the Bias, and the text of Bias could be updated by simulate WorkMode change
    workModeUpdatetxtBias();
    // 实际使用中，在速度模式下，Stop后不会马上变成0，导致显示的速度不为0，而实际已经停止
    if (workMode == MODE_SPEED) {
        uiMove->txtBias->setValue(0.0f); // 补充修改Bias的值为0
        on_txtBias_editingFinished(); // 修改Bias调用相应槽函数
    }
    txtBiasChangeManualSlider();
}
