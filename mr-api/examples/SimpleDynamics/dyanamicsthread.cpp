#include "dyanamicsthread.h"
#include <QDebug>
#include <math.h>
#define M_PI 3.14159265358979323846

DyanamicsThread::DyanamicsThread(QObject *parent, JOINT_HANDLE handle) :
    QThread(parent), m_Payload(0.0f), m_JointCurrent(0), m_JointPosition(0),
    m_JointVelocity(0), m_JointHandle(handle), stopped(true)
{
}

DyanamicsThread::~DyanamicsThread()
{
    stopped = true;
    this->wait();
}

void DyanamicsThread::stop()
{
    stopped = true;
}

void DyanamicsThread::setPayload(float m)
{
    m_Payload = m;
}

void DyanamicsThread::getJointState(float &position, float &current)
{
    position = m_JointPosition;
    current = m_JointCurrent;
}

void DyanamicsThread::run()
{
    double t0, t;
    LARGE_INTEGER m_nFreq;
    LARGE_INTEGER m_nTime;
    LARGE_INTEGER liDueTime;
    m_timer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (NULL == m_timer)
    {
        qDebug("CreateWaitableTimer failed (%d)",GetLastError());
    }

    /* arg 2 of SetWaitableTimer take 100 ns interval */
    liDueTime.QuadPart = ((long long)(-1) * 10000 * 10);  // 5ms

    if (!SetWaitableTimer(m_timer, &liDueTime, 0, NULL, NULL, FALSE))
    {
        qDebug("SetWaitableTimer failed (%d)", GetLastError());
        stopped = true;
    }
    else {
        stopped = false;
    }

    QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
    QueryPerformanceCounter(&m_nTime);   // 获取当前时间
    t0 = (double)m_nTime.QuadPart / m_nFreq.QuadPart;

    while (!stopped) {
        WaitForSingleObject(m_timer, INFINITE);
        QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期
        QueryPerformanceCounter(&m_nTime);   // 获取当前时间
        t = (double)m_nTime.QuadPart / m_nFreq.QuadPart - t0;

        jointSetCurrent(m_JointHandle, -m_Payload*sin(m_JointPosition*M_PI/180.0f), -1, NULL);
        qDebug() << "m_Payload: " << -m_Payload*sin(m_JointPosition*M_PI/180.0f);

        if (!SetWaitableTimer(m_timer, &liDueTime, 0, NULL, NULL, FALSE))
        {
            qDebug("SetWaitableTimer failed (%d)", GetLastError());
            stopped = true;
        }
        jointPoll(m_JointHandle, &m_JointPosition, &m_JointVelocity, &m_JointCurrent);
    }
}

void DyanamicsThread::start()
{
    stopped = false;
    QThread::start();
}
