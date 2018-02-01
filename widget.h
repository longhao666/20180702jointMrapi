#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Bottom;
class Move;
class SetAndHealth;
class Pid;
class OscilloScope;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void widgetAllReady();

private:
    Ui::Widget *ui;
    Bottom *bottom;
    Move *move;
    SetAndHealth *setAndHealth;
    Pid *pid;
    OscilloScope *oscilloScope;
};

#endif // WIDGET_H
