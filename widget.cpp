#include "widget.h"
#include "ui_widget.h"
#include "interface/Bottom/bottom.h"
#include "interface/Move/move.h"
#include "interface/Pid/pid.h"
#include "interface/SetAndHealth/setandhealth.h"
#include "interface/OscilloScope/oscilloscope.h"

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
    connect(this, &Widget::widgetAllReady, bottom, &Bottom::waitingForWidgetReady);
    connect(this, SIGNAL(destroyed(QObject*)), move, SLOT(ClickStopButton()));
    connect(setAndHealth, &SetAndHealth::ZeroPositionSeted, move, &Move::ClickStopButton);
//    connect(bottom, &Bottom::cmbIDJoint, this, &Widget::jointQuit);

    emit widgetAllReady();

}

Widget::~Widget()
{
    delete ui;
}
