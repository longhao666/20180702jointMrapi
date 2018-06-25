#ifndef JOINTSTATE_H
#define JOINTSTATE_H

#include <Windows.h>
#include <QThread>
#include "mrapi.h"

class JointState : public QThread
{
public:
    explicit JointState(QObject *parent = 0, JOINT_HANDLE handle = 0);
    void start();
    void stop();
protected:
    void run();
private:
    HANDLE m_timer;
    bool stopped;
    JOINT_HANDLE m_JointHandle;
};

#endif // JOINTSTATE_H
