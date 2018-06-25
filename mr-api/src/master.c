#include <math.h>
#include "master.h"

#ifndef UNUSED
#define UNUSED(arg) (void)arg
#endif

void canDispatch(Module *d, Message *msg);

CAN_HANDLE hCan[MAX_CAN_DEVICES] = { 0 };

uint8_t can1Send(Message* msg) { return canSend_driver(hCan[0], msg); }
uint8_t can2Send(Message* msg) { return canSend_driver(hCan[1], msg); }
uint8_t can3Send(Message* msg) { return canSend_driver(hCan[2], msg); }
uint8_t can4Send(Message* msg) { return canSend_driver(hCan[3], msg); }
uint8_t can5Send(Message* msg) { return 0;/* return canSend_driver(hCan[4], msg); */ }
uint8_t can6Send(Message* msg) { return 0;/* return canSend_driver(hCan[5], msg); */}

// Max 4 CAN Ports
TASK_HANDLE hReceiveTask[MAX_CAN_DEVICES] = {(TASK_HANDLE)NULL};
canSend_t hCansendHandler[MAX_CAN_DEVICES] = { can1Send, can2Send, can3Send, can4Send };

/// CAN read thread or interrupt
void _canReadISR(CAN_HANDLE h, Message* msg) {
  uint16_t cob_id = msg->cob_id;
  uint16_t id = getNodeId(cob_id);

  // assume module is a joint
  Joint* pJoint = jointSelect(id);
  if (pJoint && pJoint->basicModule){
      Module* pModule = pJoint->basicModule;
      pModule->isOnline = MODULE_ONLINE;
      canDispatch(pJoint->basicModule, msg);
  }
  else {
	  Gripper* pGripper = gripperSelect(id);
      if (pGripper && pGripper->basicModule){
          ((Module*)(pGripper->basicModule))->isOnline = MODULE_ONLINE;
          canDispatch(pGripper->basicModule, msg);
      }
  }
}

int32_t __stdcall startMaster(const char* busname, uint8_t masterId) {
  // Open and Initiallize CAN Port
  if (masterId >= MAX_CAN_DEVICES) {
	  return MR_ERROR_ILLDATA;
  }
  if (hCan[masterId] != 0) {
	  ELOG("masterId %d has been combined to CAN device HANDLE 0x%X", masterId, hCan[masterId]);
	  return MR_ERROR_ILLDATA;
  }
  hCan[masterId] = canOpen_driver(busname, "1M");

  if (CAN_NONEBUS == hCan[masterId]) {
	  return MR_ERROR_ILLDATA;   // CAN设备初始化失败
  }

  // Create and Start thread to read CAN message
  CreateReceiveTask(hCan[masterId], &hReceiveTask[masterId], _canReadISR);
  CreateLifeGuardThread();

  return MR_ERROR_OK;
}

int32_t __stdcall stopMaster(uint8_t masterId) {
  hCan[masterId] = 0;
	
  // 所有的CAN设备都被停止之后， 销毁LifeGuard线程和CAN Receive线程
  if ((hCan[0] == 0) && (hCan[1] == 0) && (hCan[2] == 0) && (hCan[3] == 0)) {
	  DestroyLifeGuardThread();
	  DestroyReceiveTask(&hReceiveTask[masterId]);
  }
  canClose_driver(hCan[masterId]);
  return MR_ERROR_OK;
}

int32_t __stdcall joinMaster(uint8_t masterId) {
  if ((hCan[masterId] == 0) || masterId >= MAX_CAN_DEVICES)  return MR_ERROR_ILLDATA;
  WaitReceiveTaskEnd(&hReceiveTask[masterId]);
  return MR_ERROR_OK;
}

