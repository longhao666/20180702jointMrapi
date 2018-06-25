#ifndef DYANAMICSTHREAD_H
#define DYANAMICSTHREAD_H

#include <Windows.h>
#include <QThread>
#include "mrapi.h"

class DyanamicsThread : public QThread
{
public:
    explicit DyanamicsThread(QObject *parent = 0, JOINT_HANDLE handle = 0);
    ~DyanamicsThread();
    void start();
    void stop();
    void setPayload(float m);
    void getJointState(float &position, float &current);
protected:
    void run();
private:
    HANDLE m_timer;
    bool stopped;
    JOINT_HANDLE m_JointHandle;

    float m_JointCurrent;
    float m_JointVelocity;
    float m_JointPosition;

    float m_Payload;
};

#endif // DYANAMICSTHREAD_H
