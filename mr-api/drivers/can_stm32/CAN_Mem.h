#ifndef __CAN_MEM_H__
#define __CAN_MEM_H__
#include "can_driver.h"
#include "RTL.h"

#define CAN_CTRL_MAX_NUM 2
#define CAN_No_ReceiveObjects 100
#define CAN_No_SendObjects 20

/* Declare mailbox, for CAN transmit messages                                */
extern uint32_t MBX_tx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_SendObjects];

/* Declare mailbox, for CAN receive messages                                 */
extern uint32_t MBX_rx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_ReceiveObjects];

Message *CAN_RxAlloc(void);
Message *CAN_TxAlloc(void);

#endif
