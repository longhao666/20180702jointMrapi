#ifndef __PCAN_BASIC_H__
#define __PCAN_BASIC_H__

#include <PCANBasic.h>

#define TIMEVAL unsigned long long
#define TIMEVAL_MAX ~(TIMEVAL)0
#define MS_TO_TIMEVAL(ms) ms*1000L
#define US_TO_TIMEVAL(us) us

#define CAN_HANDLE TPCANHandle
#define CAN_NONEBUS PCAN_NONEBUS

#if defined Linux
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#define TASK_HANDLE pthread_t

#elif defined _WINDOWS
#include <Windows.h>
#define TASK_HANDLE HANDLE
#define WATCHDOG_HANDLE HANDLE
void usleep(__int64 usec);
#endif


#endif
