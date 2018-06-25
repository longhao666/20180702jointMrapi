#include <stdlib.h>
#include <sys/time.h>
#include <asm/types.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <string.h>

#include "can_driver.h"

pthread_t LifeGuardThread;
pthread_mutex_t LifeGuard_mutex;
int32_t lifeGuardFlag = 0;
uint16_t Num_Module = 0;
uint16_t LifeGuard_Slienttime[MAX_JOINTS+MAX_GRIPPERS];
uint16_t LifeGuard_ID[MAX_JOINTS + MAX_GRIPPERS];
Callback_t LifeGuard_CB[MAX_JOINTS + MAX_GRIPPERS];

int32_t recTaskInitFlag = 0;

static void (*canTxPeriodic)(struct timeval* tv) = NULL;

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
    int fd;
    int ret;
    TPCANStatus status;
    fd_set Fds;
    CAN_HANDLE handle = (CAN_HANDLE)arg;
    Message rxMsg = Message_Initializer;
    pthread_mutex_t CanThread_mutex;

    /*get signal*/
    if(signal(SIGTERM, canReceiveLoop_signal) == SIG_ERR) {
        ELOG("signal()");
    }
    ret = pthread_mutex_init(&CanThread_mutex, NULL);
    status = CAN_GetValue(handle, PCAN_RECEIVE_EVENT, &fd, sizeof(int));
   
    if (status != PCAN_ERROR_OK) {
      char errText[256];
      CAN_GetErrorText(status, 0, errText);
      fprintf(stderr, "CAN_GetValue (PCANBasic) : %s.\n", errText);
        return NULL;
    }
    /* Watch stdin (fd 0) to see when it has input. */
    FD_ZERO(&Fds);
    FD_SET(fd, &Fds);
    recTaskInitFlag = 1;
    printf("enter canReceiveLoop\n");
    while (recTaskInitFlag && (select(fd+1, &Fds, NULL, NULL, NULL) > 0)) {
        // MSG("CAN reading loop\n");
        if (canReceive_driver(handle, &rxMsg) == 1) {
            pthread_mutex_lock(&CanThread_mutex);
            if (canRxInterruptISR)
                canRxInterruptISR(handle, &rxMsg);
            pthread_mutex_unlock(&CanThread_mutex);
        }
        else {
            //MSG("No in Message\n");
        }
        
    }
    pthread_mutex_destroy(&CanThread_mutex);

    return NULL;
}

void CreateReceiveTask(CAN_HANDLE handle, TASK_HANDLE* Thread, void* ReceiveLoopPtr)
{
    canRxInterruptISR = ReceiveLoopPtr;
    if (canRxInterruptISR == NULL) {
        ELOG("canRxInterruptISR cannot be NULL.\n");
        return;
    }

    if(pthread_create(Thread, NULL, canReceiveLoop, (void*)handle)) {
        ELOG("pthread_create()\n");
    }
    while (recTaskInitFlag == 0) {sleep(0);}
    DLOG("pthread_create()\n");
}

void DestroyReceiveTask(TASK_HANDLE* Thread)
{
    recTaskInitFlag = 0;
//    pthread_join(*Thread, NULL);
    if(pthread_kill(*Thread, SIGTERM)) {
        ELOG("pthread_kill()");
    }
}

void WaitReceiveTaskEnd(TASK_HANDLE* Thread) 
{
    if(pthread_join(*Thread, NULL)) {
        ELOG("pthread_join()");
    }
}

void* LifeGuard(void* arg)
{
    (void*)arg;
    while (lifeGuardFlag)
    {
        usleep(1000);  // 1ms
        for (uint16_t i = 0; i < Num_Module; i++) {
            pthread_mutex_lock(&LifeGuard_mutex);
            LifeGuard_Slienttime[i]++;
            pthread_mutex_unlock(&LifeGuard_mutex);
            if (LifeGuard_Slienttime[i] >= 100) {  // 100ms
                LifeGuard_CB[i](LifeGuard_ID[i], 0, (void*)&LifeGuard_Slienttime[i]);
            }
        }

    }
    pthread_mutex_destroy(&LifeGuard_mutex);
    return NULL;
}

void CreateLifeGuardThread()
{
    unsigned long thread_id = 0;

    if (lifeGuardFlag == 0)
    {
        pthread_mutex_init(&LifeGuard_mutex, NULL);
        lifeGuardFlag = 1;
        if(pthread_create(&LifeGuardThread, NULL, LifeGuard, (void*)0)) {
            ELOG("pthread_create()\n");
        }
    }
}

void DestroyLifeGuardThread()
{
    lifeGuardFlag = 0;
    pthread_join(LifeGuardThread, NULL);
}

void RegisterLifeGuard(uint16_t moduleId, Callback_t cb)
{
    if (Num_Module == MAX_GRIPPERS + MAX_JOINTS) return;
    pthread_mutex_lock(&LifeGuard_mutex);
    LifeGuard_CB[Num_Module] = cb;
    LifeGuard_ID[Num_Module] = moduleId;
    LifeGuard_Slienttime[Num_Module] = 0;
    pthread_mutex_unlock(&LifeGuard_mutex);

    Num_Module++;
}

void UnregisterLifeGuard(uint16_t moduleId)
{
    uint16_t i;
    for (i = 0; i < Num_Module; i++) {
        if (LifeGuard_ID[i] == moduleId) break;
    }

    pthread_mutex_lock(&LifeGuard_mutex);
    for (; i < Num_Module - 1; i++) {  //
        LifeGuard_CB[i] = LifeGuard_CB[i+1];
        LifeGuard_ID[i] = LifeGuard_ID[i + 1];
        LifeGuard_Slienttime[i] = LifeGuard_Slienttime[i + 1];
    }
    pthread_mutex_unlock(&LifeGuard_mutex);

    Num_Module--;
}

void ResetLifeGuard(uint16_t moduleId)
{
    uint16_t i;

    pthread_mutex_lock(&LifeGuard_mutex);
    for (i = 0; i < Num_Module; i++) {
        if (LifeGuard_ID[i] == moduleId) break;
    }
    LifeGuard_Slienttime[i] = 0;
    pthread_mutex_unlock(&LifeGuard_mutex);
}

