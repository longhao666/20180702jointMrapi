#include "jointstate.h"
#include <QDebug>
#include <math.h>

JointState::JointState(QObject *parent, JOINT_HANDLE handle) :
    QThread(parent),
    m_JointHandle(handle), stopped(true)
{
}

void JointState::stop()
{
    stopped = true;
}

void JointState::run()
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

        double q = 10*cos(t) + 5*cos(2*t);
        double v = -10*sin(t) - 10*sin(2*t);
        jointPush(m_JointHandle, q, v);
//        qDebug("time: %.4f\t", ((double)m_nTime.QuadPart / m_nFreq.QuadPart));

        if (!SetWaitableTimer(m_timer, &liDueTime, 0, NULL, NULL, FALSE))
        {
            qDebug("SetWaitableTimer failed (%d)", GetLastError());
            stopped = true;
        }
//        jointPoll(m_JointHandle, 0,0,0);
    }
}

void JointState::start()
{
    stopped = false;
    QThread::start();
}
