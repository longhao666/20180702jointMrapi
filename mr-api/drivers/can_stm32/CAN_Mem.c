#include "CAN_Mem.h"

/* Declare mailbox, for CAN transmit messages                                */
uint32_t MBX_tx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_SendObjects];

/* Declare mailbox, for CAN receive messages                                 */
uint32_t MBX_rx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_ReceiveObjects];

uint16_t RxIndex = 0;
Message CAN_RxPool[CAN_No_ReceiveObjects];

uint16_t TxIndex = 0;
Message CAN_TxPool[CAN_No_SendObjects];

Message *CAN_RxAlloc(void)
{
  Message *ptr = &CAN_RxPool[RxIndex];
  RxIndex = (RxIndex+1)%CAN_No_ReceiveObjects;
  
  return ptr;
}

Message *CAN_TxAlloc(void)
{
  Message *ptr = &CAN_TxPool[TxIndex];
  TxIndex = (TxIndex+1)%CAN_No_SendObjects;
  return ptr;
}

