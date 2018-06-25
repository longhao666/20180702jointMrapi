#include <pcap/pcap.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include "can_driver.h"

#define RELAY_TYPE (uint16_t)0x1234

timer_t LifeGuardThread;
pthread_mutex_t LifeGuard_mutex;
int32_t lifeGuardFlag = 0;
uint16_t Num_Module = 0;
uint16_t LifeGuard_Slienttime[MAX_JOINTS+MAX_GRIPPERS];
uint16_t LifeGuard_ID[MAX_JOINTS + MAX_GRIPPERS];
Callback_t LifeGuard_CB[MAX_JOINTS + MAX_GRIPPERS];


pcap_t* device = NULL;
pthread_t thread = (pthread_t)NULL;
static void (*canRxInterruptISR)(CAN_HANDLE h, Message* msg) = NULL;
int loopFlag = 0;

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet);

void canReceiveLoop_signal(int sig)
{
    if (sig == SIGTERM){
       pthread_exit(NULL);
    }
}

void *canReceiveLoop(void *handle)
{
    signal(SIGTERM, canReceiveLoop_signal);
    /* wait loop forever */

    loopFlag = 1;
    pcap_loop(device, -1, getPacket, NULL);
    pthread_exit(NULL);
    return NULL;
}

void CreateReceiveTask(CAN_HANDLE fd0, TASK_HANDLE* Thread, void* ReceiveLoopPtr)
{
    if (thread != (pthread_t)NULL) return;
    canRxInterruptISR = ReceiveLoopPtr;
    pthread_create(Thread, NULL, (void *(*)(void *))canReceiveLoop, NULL);
    while(!loopFlag){sleep(0);}
    thread = *Thread;
}

void DestroyReceiveTask(TASK_HANDLE* Thread)
{
    pthread_kill(*Thread, SIGTERM);
}

void WaitReceiveTaskEnd(TASK_HANDLE* Thread)
{
    pthread_join(*Thread, NULL);
}

uint8_t canChangeBaudRate_driver(CAN_HANDLE fd, char* baud)
{

}

CAN_HANDLE canOpen_driver(const char* busno, const char* baud)
{
    char errBuf[PCAP_ERRBUF_SIZE];
    char devStr[256];
    int canId;

    (void)baud;

    sscanf(busno, "%[^-]-%d", devStr, &canId);

    // ethernet has been initiallized
    if (device != NULL) return canId;
    printf("devStr:%s\n",devStr);
    device = pcap_create(devStr, errBuf);
    if (!device) {
        printf("error: %s\n", errBuf);
        return 0;
    }

    pcap_set_immediate_mode(device, 1);
    pcap_set_buffer_size(device, 10);
    pcap_activate(device);

    return (char)canId;
}

void canReset_driver(CAN_HANDLE handle, char* baud)
{

}

uint8_t canSend_driver(CAN_HANDLE fd0, Message const *m)
{
    char errBuf[PCAP_ERRBUF_SIZE];
    char *sendBuf;

    sendBuf = malloc(15 + sizeof(Message));
    for (int i = 0; i < 12; i++) sendBuf[i] = 0xFF;
    sendBuf[12] = (uint8_t)RELAY_TYPE;
    sendBuf[13] = (uint8_t)(RELAY_TYPE>>8);
    sendBuf[14] = fd0;
    memcpy(sendBuf + 15, (void*)m, sizeof(Message));
    if (pcap_inject(device, (const void*)sendBuf, 15 + sizeof(Message)) == -1) {
        pcap_perror(device, errBuf);
        printf("Couldn't send frame: %s\n", errBuf);
        return 2;
    }
    return 0;
}

uint8_t canReceive_driver(CAN_HANDLE fd0, Message *m) {

}

int canClose_driver(CAN_HANDLE handle)
{
    pcap_close(device);
}

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
{
    Message m;

    if((packet[12] == (uint8_t)RELAY_TYPE) && (packet[13] == (uint8_t)(RELAY_TYPE>>8))) {
        uint8_t canId = packet[14];
        memcpy(&m, packet + 15, sizeof(Message));
        if (canRxInterruptISR) canRxInterruptISR(canId, &m);
    }
}

#define maxval(a,b) ((a>b)?a:b)
timer_t startTimerLoop(int32_t hz, void* timer_notify)
{
    timer_t timerid;
    struct itimerspec timerValues;
    struct sigevent sigev;
    float val;
    if (hz == 0) val = 0.0f;
    else val = 1000000.0/(float)hz;
    long tv_nsec = 1000 * (maxval((int)val,1)%1000000);
    time_t tv_sec = val/1000000;

    memset (&sigev, 0, sizeof (struct sigevent));
    sigev.sigev_value.sival_int = 0;
    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_attributes = NULL;
    sigev.sigev_notify_function = timer_notify;

    if(timer_create (CLOCK_REALTIME, &sigev, &timerid) == 0) {
//        printf("timer created.\n");
    }

    timerValues.it_value.tv_sec = tv_sec;
    timerValues.it_value.tv_nsec = tv_nsec;
    timerValues.it_interval.tv_sec = tv_sec;
    timerValues.it_interval.tv_nsec = tv_nsec;

    if(timer_settime (timerid, 0, &timerValues, NULL) == -1) {
        perror("fail to timer_settime");
    }
    return timerid;
}

void LifeGuard(void)
{
    for (uint16_t i = 0; i < Num_Module; i++) {
        pthread_mutex_lock(&LifeGuard_mutex);
        LifeGuard_Slienttime[i]++;
        pthread_mutex_unlock(&LifeGuard_mutex);
        if (LifeGuard_Slienttime[i] == 100) {  // 100ms
            if (LifeGuard_CB[i]) LifeGuard_CB[i](LifeGuard_ID[i], 0, (void*)&LifeGuard_Slienttime[i]);
            LifeGuard_Slienttime[i] = 0;
        }
    }
}

void CreateLifeGuardThread()
{
    if (lifeGuardFlag == 0)
    {
        pthread_mutex_init(&LifeGuard_mutex, NULL);
        lifeGuardFlag = 1;
        LifeGuardThread = startTimerLoop(1000, LifeGuard);
    }
}

void DestroyLifeGuardThread()
{
    lifeGuardFlag = 0;
    timer_delete(LifeGuardThread);
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
    if (i == Num_Module) return;

    pthread_mutex_lock(&LifeGuard_mutex);
    for (; i < Num_Module - 1; i++) {  //
        LifeGuard_CB[i] = LifeGuard_CB[i + 1];
        LifeGuard_ID[i] = LifeGuard_ID[i + 1];
        LifeGuard_Slienttime[i] = LifeGuard_Slienttime[i + 1];
    }
    pthread_mutex_unlock(&LifeGuard_mutex);

    Num_Module--;
}

void ResetLifeGuard(uint16_t moduleId)
{
    uint16_t i;
    for (i = 0; i < Num_Module; i++) {
        if (LifeGuard_ID[i] == moduleId) break;
    }
    if (i == Num_Module) return;

    pthread_mutex_lock(&LifeGuard_mutex);
    LifeGuard_Slienttime[i] = 0;
    pthread_mutex_unlock(&LifeGuard_mutex);
}

