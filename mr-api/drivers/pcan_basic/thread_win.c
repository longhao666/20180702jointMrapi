#include <stdlib.h>
#include <sys/timeb.h>
#include <signal.h>
#include <math.h>
#include <string.h>
#include <Windows.h>

#include "can_driver.h"

HANDLE LifeGuardThread;
CRITICAL_SECTION LifeGuard_mutex;
int32_t lifeGuardFlag = 0;
uint16_t Num_Module = 0;
uint16_t LifeGuard_Slienttime[MAX_JOINTS+MAX_GRIPPERS];
uint16_t LifeGuard_ID[MAX_JOINTS + MAX_GRIPPERS];
Callback_t LifeGuard_CB[MAX_JOINTS + MAX_GRIPPERS];

int32_t recTaskInitFlag = 0;

void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

void canReceiveLoop_signal(int sig)
{
}
/* We assume that ReceiveLoop_task_proc is always the same */
static void (*canRxInterruptISR)(CAN_HANDLE h, Message* msg) = NULL;
/**
 * Enter in realtime and start the CAN receiver loop
 * @param port
 */
void* canReceiveLoop(void* arg)
{
	uint8_t recRet;
    TPCANStatus status;
    CAN_HANDLE handle = (CAN_HANDLE)arg;
    HANDLE hEvent = NULL;
    Message rxMsg = Message_Initializer;
	CRITICAL_SECTION CanThread_mutex;

	InitializeCriticalSection(&CanThread_mutex);
	hEvent = CreateEvent(NULL, FALSE, FALSE, L"ReceiveEvent");

    status = CAN_SetValue(handle, PCAN_RECEIVE_EVENT, &hEvent, sizeof(HANDLE));
    if (status != PCAN_ERROR_OK) {
        char strMsg[256];
        CAN_GetErrorText(status, 0, strMsg);
        ELOG("CAN_SetValue : %s",strMsg);
        return NULL;
    }
	//Init FLAG is set to avoid message coming while event hasn't set
	recTaskInitFlag = 1;
    while (recTaskInitFlag) {
        if ( WAIT_OBJECT_0 == WaitForSingleObject(hEvent, INFINITE)) {
			//first of all, you are creating an auto-reset event, so you don't need to call ResetEvent() on your handle.
			//ResetEvent(hEvent);
            do {
				recRet = canReceive_driver(handle, &rxMsg);
				EnterCriticalSection(&CanThread_mutex);
                if ((recRet == 1) && canRxInterruptISR)
                    canRxInterruptISR(handle, &rxMsg);
				LeaveCriticalSection(&CanThread_mutex);
			} while (recRet == 1);
			if (recRet == 2) {
				ILOG("Please check if there is any module on the bus.CAN bus 0x%x will be reset with baudrate of 1M", handle);
				CAN_Reset(handle);
				canReset_driver(handle, "1M");
				CAN_SetValue(handle, PCAN_RECEIVE_EVENT, &hEvent, sizeof(HANDLE));
			}
        }
    }
   DeleteCriticalSection(&CanThread_mutex);

    return NULL;
}

void CreateReceiveTask(CAN_HANDLE handle, TASK_HANDLE* Thread, void* ReceiveLoopPtr)
{
    unsigned long thread_id = 0;

	canRxInterruptISR = ReceiveLoopPtr;
	if (canRxInterruptISR == NULL) {
        ELOG("canRxInterruptISR cannot be NULL.\n");
        return;
    }

    *Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)canReceiveLoop, (void*)handle, 0, &thread_id);

	while (recTaskInitFlag != 1) { Sleep(0); }

    ILOG("Receive Task created");
}

void DestroyReceiveTask(TASK_HANDLE* Thread)
{
	recTaskInitFlag = 0;
	if (WaitForSingleObject(*Thread, 1000) == WAIT_TIMEOUT)
	{
		TerminateThread(*Thread, -1);
	}
	CloseHandle(*Thread);
}

void WaitReceiveTaskEnd(TASK_HANDLE* Thread) 
{
	WaitForSingleObject(*Thread, INFINITE);
}

void* LifeGuard(void* arg)
{
	while (lifeGuardFlag)
	{
		usleep(1000);  // 1ms
		for (uint16_t i = 0; i < Num_Module; i++) {
			EnterCriticalSection(&LifeGuard_mutex);
			LifeGuard_Slienttime[i]++;
			LeaveCriticalSection(&LifeGuard_mutex);
			if (LifeGuard_Slienttime[i] >= 100) {  // 100ms
				LifeGuard_CB[i](LifeGuard_ID[i], 0, (void*)&LifeGuard_Slienttime[i]);
			}
		}

	}
	DeleteCriticalSection(&LifeGuard_mutex);
    return NULL;
}

void CreateLifeGuardThread()
{
	unsigned long thread_id = 0;

	if (lifeGuardFlag == 0)
	{
		InitializeCriticalSection(&LifeGuard_mutex);
		lifeGuardFlag = 1;
		LifeGuardThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LifeGuard, (void*)0, 0, &thread_id);
	}
}

void DestroyLifeGuardThread()
{
	lifeGuardFlag = 0;
	if (WaitForSingleObject(LifeGuardThread, 1000) == WAIT_TIMEOUT)
	{
		TerminateThread(LifeGuardThread, -1);
	}
	CloseHandle(LifeGuardThread);
}

void RegisterLifeGuard(uint16_t moduleId, Callback_t cb)
{
	if (Num_Module == MAX_GRIPPERS + MAX_JOINTS) return;
	EnterCriticalSection(&LifeGuard_mutex);
	LifeGuard_CB[Num_Module] = cb;
	LifeGuard_ID[Num_Module] = moduleId;
	LifeGuard_Slienttime[Num_Module] = 0;
	LeaveCriticalSection(&LifeGuard_mutex);

	Num_Module++;
}

void UnregisterLifeGuard(uint16_t moduleId)
{
	uint16_t i;
	for (i = 0; i < Num_Module; i++) {
		if (LifeGuard_ID[i] == moduleId) break;
	}

	EnterCriticalSection(&LifeGuard_mutex);
	for (; i < Num_Module - 1; i++) {  // ÒÆÎ»
		LifeGuard_CB[i] = LifeGuard_CB[i+1];
		LifeGuard_ID[i] = LifeGuard_ID[i + 1];
		LifeGuard_Slienttime[i] = LifeGuard_Slienttime[i + 1];
	}
	LeaveCriticalSection(&LifeGuard_mutex);

	Num_Module--;
}

void ResetLifeGuard(uint16_t moduleId)
{
	uint16_t i;
	EnterCriticalSection(&LifeGuard_mutex);
	for (i = 0; i < Num_Module; i++) {
		if (LifeGuard_ID[i] == moduleId) break;
	}

	LifeGuard_Slienttime[i] = 0;
	LeaveCriticalSection(&LifeGuard_mutex);
}

