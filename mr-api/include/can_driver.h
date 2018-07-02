#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__

#include <stdio.h>
#include <stdint.h>

#include "mrapi.h"  //should before logger.h
#include "logger.h"

#if defined PCAN_BASIC
#include "pcan_basic.h"
#define MIN_TV 1
#endif

#if defined ECAN_BASIC
#include "ecan_basic.h"
#define MIN_TV 1
#endif

#if defined CAN_STM32
#include "can_stm32.h"
#define MIN_TV 1000
#endif

#define delay_us(n) usleep(n)

/** 
 * @brief The CAN message structure 
 * @ingroup can
 */
typedef struct {
  uint16_t cob_id; /**< message's ID */
  uint8_t rtr;     /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  uint8_t len;     /**< message's length (0 to 8) */
  uint8_t data[8]; /**< message's datas */
} Message;

#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}

typedef uint8_t (*canSend_t)(Message *);

static inline void print_message(Message* m)
{
    int i;
    char msg[256];
    sprintf(msg, "%04X ", m->cob_id);

    for (i = 0 ; i < m->len ; i++)
        sprintf(msg, "%s %02X",msg, m->data[i]);
    sprintf(msg, "%s\n", msg);
    RLOG("%s",msg);
}

#ifdef __cplusplus
extern "C"
{
#endif

void CreateReceiveTask(CAN_HANDLE fd0, TASK_HANDLE* Thread, void* ReceiveLoopPtr);
void DestroyReceiveTask(TASK_HANDLE* Thread);
void WaitReceiveTaskEnd(TASK_HANDLE* Thread);

void CreateLifeGuardThread(void);
void DestroyLifeGuardThread(void);
void RegisterLifeGuard(uint16_t moduleId, Callback_t cb);
void UnregisterLifeGuard(uint16_t moduleId);
void ResetLifeGuard(uint16_t moduleId);

uint8_t canChangeBaudRate_driver(CAN_HANDLE fd, char* baud);
CAN_HANDLE canOpen_driver(const char* busno, const char* baud);
void canReset_driver(CAN_HANDLE handle, char* baud);
uint8_t canSend_driver(CAN_HANDLE fd0, Message const *m);
uint8_t canReceive_driver(CAN_HANDLE fd0, Message *m);
int canClose_driver(CAN_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif
