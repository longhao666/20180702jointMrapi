#include "widget.h"
#include "ui_widget.h"
#include "bottom.h"
#include "move.h"
#include "pid.h"
#include "setandhealth.h"
#include "oscilloscope.h"
#include <QDebug>

JOINT_HANDLE m_joint = NULL;
JOINT_HANDLE m_joint_copy = NULL;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    bottom = new Bottom;
    ui->bottomHorizontalLayout->addWidget(bottom);
    move = new Move;
//    ui->moveHorizontalLayout->addWidget(move);
    setAndHealth = new SetAndHealth;
//    ui->setAndHealthHorizontalLayout->addWidget(setAndHealth);
    pid = new Pid;
    ui->verticalLayout->addWidget(move);
    ui->verticalLayout->addWidget(pid);
    ui->verticalLayout->addWidget(setAndHealth);

//    ui->pidHorizontalLayout->addWidget(pid);
    oscilloScope = new OscilloScope;
    ui->oscilloscopeHorizontalLayout->addWidget(oscilloScope);

    connect(bottom, &Bottom::cmbIDChanged, pid, &Pid::pidInit);
    connect(bottom, &Bottom::cmbIDChanged, move, &Move::moveInit);
    connect(bottom, &Bottom::cmbIDChanged, setAndHealth, &SetAndHealth::SetAndHealthIint);
    connect(bottom, &Bottom::cmbIDChanged, oscilloScope, &OscilloScope::OscilloScopeInitialize);
//    connect(this, &Widget::widgetAllReady, bottom, &Bottom::waitingForWidgetReady);
    connect(this, SIGNAL(destroyed(QObject*)), move, SLOT(ClickStopButton()));
    connect(setAndHealth, &SetAndHealth::ZeroPositionSeted, move, &Move::ClickStopButton);
    connect(bottom, &Bottom::cmbIDJoint, this, &Widget::jointQuit);
    connect(bottom, &Bottom::signalRecoverBotton, move, &Move::slotRecoverButton);

//    emit widgetAllReady();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::jointQuit()
{
    m_joint_copy = m_joint;
    m_joint = NULL;
    int re = jointDown(m_joint_copy);
    qDebug() << "m_joint =" << m_joint << re;
}
