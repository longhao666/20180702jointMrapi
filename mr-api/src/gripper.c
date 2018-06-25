#include <stdlib.h>
#include <string.h>
#include "module.h"
#include "gripper.h"

// 内存控制表宏定义
#define CMDMAP_LEN				80			//内存控制表总长度（半字单位）

//内存控制表读写权限
uint8_t gripper_accessType[5][16] =
{
	{//0x0*字段
		RO,	//字头(MODULE_TYP<<8)|0x50
		RW,	//手抓ID
		RO,	//驱动器型号：0x80-Gripper
		RO,	//固件版本
		RO,	//错误代码
		RO,	//系统电压（0.01V）,暂不可用
		RO,	//系统温度（0.1℃）,暂不可用
		RW,	//舵机LED:高8位-舵机ID，低8位-亮灭
		RW,	//RS485总线波特率(串口波特率)
		RW,	//CAN总线波特率
		RW,	//使能舵机力矩输出
		RW,	//上电时使能舵机力矩输出
		RW,	//保存数据到FLASH标志（自动清零）
		NO_ACCESS,	//保留
		RW,	//设置零点
		RW,	//清除错误标志
	},
	{//0x1*字段
		RW,	//更新舵机的信息，相应位置写1，自动清除
		RO,	//左舵机当前位置
		RO,	//右舵机当前位置
		RO,	//左舵机当前速度
		RO,	//右舵机当前速度
		RO,	//左舵机当前负载
		RO,	//右舵机当前负载
		RO,	//左舵机当前电压（0.1V）
		RO,	//右舵机当前电压（0.1V）
		RO,	//左舵机当前温度（1℃）
		RO,	//右舵机当前温度（1℃）
		RO,	//系统电流 mA
		RO,	//加速度x
		RO,	//加速度y
		RO,	//加速度z
	},
	{//0x2*字段				
		RW,	//工作模式：1-开合，2-位置
		RW,	//开合大小
		RW,	//手指开合状态
		RW,	//左舵机零点位置
		RW,	//右舵机零点位置
		RW,	//左舵机速度限制
		RW,	//右舵机速度限制
		RW,	//左舵机力矩限制
		RW,	//右舵机力矩限制
		RW,	//左舵机目标位置
		RW,	//右舵机目标位置
	},
	{//0x3*字段				
		RW,	//最大限制转矩(舵机ROM)
		RW,	//最大限制转矩(舵机RAM)
		RW,	//转动速度
		RW,	//位置误差
		RW,	//加速度
		RW,	//Punch
		RW,	//Status返回延迟
		RW,	//左舵机顺时针角度限制
		RW,	//左舵机逆时针角度限制
		RW,	//右舵机顺时针角度限制
		RW,	//右舵机逆时针角度限制
		RW,	//温度限制
	},
	{//0x4*字段				
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
		RW,	//舵机任意读写命令
	},
};

extern canSend_t hCansendHandler[MAX_CAN_DEVICES];
Gripper* gripperStack[MAX_GRIPPERS];    // online gripper stack
uint16_t gripperNbr = 0;

Gripper* gripperConstruct(uint16_t id, canSend_t canSend) {
	uint16_t indexMap[4] = { SYS_POSITION_LEFT, SYS_LOAD_LEFT, SYS_POSITION_RIGHT, SYS_LOAD_RIGHT };
	Gripper* pGripper = (Gripper*)malloc(sizeof(Gripper));
	Module* pModule;
	pGripper->basicModule = (Module*)malloc(sizeof(Module));
	pModule = pGripper->basicModule;
	pModule->memoryLen = CMDMAP_LEN;
	pModule->memoryTable = (uint16_t*)calloc(CMDMAP_LEN, sizeof(uint16_t));
	pModule->readFlag = (int16_t*)calloc(CMDMAP_LEN, sizeof(uint16_t));
	pModule->writeFlag = (int16_t*)calloc(CMDMAP_LEN, sizeof(uint16_t));
	memset(pModule->memoryTable, 0, CMDMAP_LEN * sizeof(uint16_t));
	pModule->readDoneCb = (Callback_t*)calloc(CMDMAP_LEN, sizeof(Callback_t));
	pModule->writeDoneCb = (Callback_t*)calloc(CMDMAP_LEN, sizeof(Callback_t));
	pModule->accessType = (uint8_t*)gripper_accessType;

	pModule->memoryTable[SYS_ID] = id;
	pGripper->gripperId = (uint16_t*)&(pModule->memoryTable[SYS_ID]);
	pModule->moduleId = pGripper->gripperId;
	pModule->canSend = canSend;

	pGripper->gripperType = (uint16_t*)&(pModule->memoryTable[SYS_MODEL_TYPE]);

	pModule->isOnline = MODULE_OFFLINE;

	setSyncReceiveMap(pGripper->basicModule, indexMap);

	return pGripper;
}

int32_t gripperDestruct(Gripper* pGripper) {
	Module* pModule;
	if (pGripper) {
		pModule = (Module*)pGripper->basicModule;
		if (pModule->memoryTable)
			free(pModule->memoryTable);
		if (pModule->readDoneCb)
			free(pModule->readDoneCb);
		if (pModule->writeDoneCb)
			free(pModule->writeDoneCb);
		if (pModule->readFlag)
			free(pModule->readFlag);
		if (pModule->writeFlag)
			free(pModule->writeFlag);
		free(pModule);
        pModule = NULL;
		free(pGripper);
		return MR_ERROR_OK;
	}
	return MR_ERROR_ILLDATA;
}

GRIPPER_HANDLE __stdcall gripperSelect(uint16_t id) {
	uint16_t i;
	for (i = 0; i < gripperNbr; i++) {
		if (id == *(gripperStack[i]->gripperId))
			return (GRIPPER_HANDLE)gripperStack[i];
	}

	return NULL;
}

int32_t __stdcall gripperPush(GRIPPER_HANDLE h, float left_pos, float right_pos) {
	Gripper* pGripper = (Gripper*)h;
	int32_t buf[2] = { 0 };
	if (!pGripper) {
		return MR_ERROR_ILLDATA;
	}
	buf[0] = (int32_t)(left_pos);
	buf[1] = (int32_t)(right_pos);

	writeSyncMsg(pGripper->basicModule, 0x200, (void*)buf);

	return MR_ERROR_OK;
}

/// 从内存表获取实际位置和实际电流
int32_t __stdcall gripperPoll(GRIPPER_HANDLE h, float* left_pos, float* right_pos, float* left_torque, float* right_torque) {
	Gripper* pGripper = (Gripper*)h;
	Module* pModule;
	int32_t temp;
	if (!pGripper) {
		return MR_ERROR_ILLDATA;
	}
	pModule = (Module*)pGripper->basicModule;
	if (left_pos) {
		memcpy(&temp, &(pModule->memoryTable[SYS_POSITION_LEFT]), 2);
		*left_pos = (float)temp; // degree
	}
	if (right_pos) {
		memcpy(&temp, &(pModule->memoryTable[SYS_POSITION_RIGHT]), 2);
		*right_pos = (float)temp; // degree
	}
	if (left_torque) {
		memcpy(&temp, &(pModule->memoryTable[SYS_LOAD_LEFT]), 2);
		*left_torque = (float)temp; // degree
	}
	if (right_torque) {
		memcpy(&temp, &(pModule->memoryTable[SYS_LOAD_RIGHT]), 2);
		*right_torque = (float)temp; // degree
	}
    if (pModule->isOnline == MODULE_OFFLINE) {
        return MR_ERROR_OFFLINE;
    }

	return MR_ERROR_OK;
}

int32_t gripperLifeGuard(uint16_t id, uint16_t index, void* args)
{
	Gripper* h = (Gripper*)gripperSelect(id);
	Module* pModule = (Module*)(h->basicModule);
	uint16_t getId;
	uint16_t silentTime = *(uint16_t*)args;
    if (silentTime == 100) {
        gripperGetId(h, &getId, -1, NULL);
		pModule->isOnline = MODULE_OFFLINE;
	}
	return MR_ERROR_OK;
}


GRIPPER_HANDLE __stdcall gripperUp(uint16_t gripperId, uint8_t masterId) {
	int32_t res, i = 0;
	Gripper* pGripper = gripperConstruct(gripperId, (canSend_t)hCansendHandler[masterId]);

	if (gripperNbr >= MAX_GRIPPERS) {
		ELOG("Gripper Stack Overflow");
		return NULL;
	}
	else {
		if (pGripper != gripperSelect(*(pGripper->gripperId)))
			gripperStack[gripperNbr++] = pGripper; // push into stack
		else return (GRIPPER_HANDLE)pGripper; // already in the stack
	}
	res = gripperGetType(pGripper, NULL, 5000, NULL);
	if ((res == 0) && isGripperType(*(pGripper->gripperType))) {
        RegisterLifeGuard(*(pGripper->gripperId), gripperLifeGuard);
        ((Module*)(pGripper->basicModule))->isOnline = MODULE_ONLINE;
		return (GRIPPER_HANDLE)pGripper;
	}
	gripperStack[--gripperNbr] = NULL; // delete from stack
	gripperDestruct(pGripper);
	return NULL;
}

int32_t __stdcall gripperDown(GRIPPER_HANDLE h) {
    uint16_t i, id;
	Gripper* pGripper = h;
	if (!gripperNbr) {
		ELOG("Gripper Stack Underflow");
		return MR_ERROR_ILLDATA;
	}
	if (!pGripper) {
		ELOG("Gripper is NULL");
		return MR_ERROR_ILLDATA;
	}
    id = *(pGripper->gripperId);
    UnregisterLifeGuard(id);
    ((Module*)(pGripper->basicModule))->isOnline = MODULE_OFFLINE;

	for (i = 0; i < gripperNbr; i++) {
		if (pGripper == gripperStack[i])
			break;
	}
	for (; i < gripperNbr - 1; i++) {
		gripperStack[i] = gripperStack[i + 1];
	}
	gripperStack[--gripperNbr] = NULL;
	gripperDestruct(pGripper);
	return MR_ERROR_OK;
}

/// waiting for n us, if return MR_ERROR_OK, id will be stored in pGripper
int32_t __stdcall gripperGet(uint8_t index, uint8_t datLen, Gripper* pGripper, void* data, int32_t timeout, Callback_t callBack) { //us
	Module* pModule;
    int32_t ret;
    if (pGripper == NULL) {
		return MR_ERROR_ILLDATA;
	}
	pModule = pGripper->basicModule;
	if (timeout == 0) {
		if (data) {
			memcpy(data, (void*)(pModule->memoryTable + index), datLen);
		}
		return MR_ERROR_OK;
	}
	else if (timeout < -1) {
		return MR_ERROR_ILLDATA;
    } else {
        ret = moduleGet(index, datLen, pModule, data, timeout, callBack);
    }

    if (pModule->isOnline == MODULE_OFFLINE) {
        return MR_ERROR_OFFLINE;
    }
    return ret;
}

/// 如果timeout为infinite且callBack为空，则调用无返回的写函数writeEntryNR
int32_t __stdcall gripperSet(uint8_t index, uint8_t datLen, Gripper* pGripper, void* data, int32_t timeout, Callback_t callBack) { //us
	Module* pModule;
    int32_t ret;
    if (pGripper == NULL) {
		return MR_ERROR_ILLDATA;
	}
	pModule = pGripper->basicModule;
    ret = moduleSet(index, datLen, pModule, data, timeout, callBack);

    if (pModule->isOnline == MODULE_OFFLINE) {
        return MR_ERROR_OFFLINE;
    }

    return ret;
}

/// waiting for n us, if return 0, id will be stored in pGripper
int32_t __stdcall gripperGetId(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) { //us
	return gripperGet(SYS_ID, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetType(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) { //us
	return gripperGet(SYS_MODEL_TYPE, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetError(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_ERROR, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetVoltage(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_VOLTAGE, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetTemp(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_TEMP, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetBaudrate(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_BAUDRATE_CAN, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetPosition(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_POSITION_LEFT, 4, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetSpeed(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_SPEED_LEFT, 4, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetTorque(GRIPPER_HANDLE pGripper, int16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(SYS_LOAD_LEFT, 4, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetMode(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(TAG_MODE, 2, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperGetMaxSpeed(GRIPPER_HANDLE pGripper, uint16_t* data, int32_t timeout, Callback_t callBack) {
	return gripperGet(TAG_SPEED_LEFT, 4, (Gripper*)pGripper, data, timeout, callBack);
}

int32_t __stdcall gripperSetID(GRIPPER_HANDLE pGripper, uint16_t id, int32_t timeout, Callback_t callBack) {
	return gripperSet(SYS_ID, 2, (Gripper*)pGripper, (void*)&id, timeout, callBack);
}

int32_t __stdcall gripperSetBaudrate(GRIPPER_HANDLE pGripper, uint16_t baud, int32_t timeout, Callback_t callBack) { //us
	return gripperSet(SYS_BAUDRATE_CAN, 2, (Gripper*)pGripper, (void*)&baud, timeout, callBack);
}

int32_t __stdcall gripperSetEnable(GRIPPER_HANDLE pGripper, uint16_t isEnable, int32_t timeout, Callback_t callBack) {
	return gripperSet(SYS_ENABLE_TORQUE, 2, (Gripper*)pGripper, (void*)&isEnable, timeout, callBack);
}

int32_t __stdcall gripperSetSave2Flash(GRIPPER_HANDLE pGripper, int32_t timeout, Callback_t callBack) {
	uint16_t isEnable = 1;
	return gripperSet(SYS_SAVE_TO_FLASH, 2, (Gripper*)pGripper, (void*)&isEnable, timeout, callBack);
}

int32_t __stdcall gripperSetZero(GRIPPER_HANDLE pGripper, int32_t timeout, Callback_t callBack)
{
	uint16_t isEnable = 1;
	return gripperSet(SYS_SET_ZERO_POS, 2, (Gripper*)pGripper, (void*)&isEnable, timeout, callBack);
}

int32_t __stdcall gripperSetClearError(GRIPPER_HANDLE pGripper, int32_t timeout, Callback_t callBack) {
	uint16_t isEnable = 1;
	return gripperSet(SYS_CLEAR_ERROR, 2, (Gripper*)pGripper, (void*)&isEnable, timeout, callBack);
}

int32_t __stdcall gripperSetMode(GRIPPER_HANDLE pGripper, gripperMode_t mode, int32_t timeout, Callback_t callBack) { //us
	if (isGripperMode(mode)) {
		return gripperSet(TAG_MODE, 2, (Gripper*)pGripper, (void*)&mode, timeout, callBack);
	}
	return MR_ERROR_ILLDATA;
}

int32_t __stdcall gripperSetUpdate(GRIPPER_HANDLE pGripper, uint16_t mask, int32_t timeout, Callback_t callBack) { //us
	return gripperSet(SYS_GET_STATE_MASK, 2, (Gripper*)pGripper, (void*)&mask, timeout, callBack);
}

int32_t __stdcall gripperSetTorque(GRIPPER_HANDLE pGripper, int16_t torq_left, int16_t torq_right, int32_t timeout, Callback_t callBack) {
	int16_t buf[2] = { torq_left, torq_right };
	return gripperSet(TAG_TORQUE_LEFT, 4, (Gripper*)pGripper, (void*)buf, timeout, callBack);
}

int32_t __stdcall gripperSetSpeed(GRIPPER_HANDLE pGripper, float speed_left, float speed_right, int32_t timeout, Callback_t callBack) {
	int16_t buf[2];

	buf[0] = (uint16_t)(speed_left / 360.0f*60.0f / 0.229f);
	buf[1] = (uint16_t)(speed_right / 360.0f*60.0f / 0.229f);

	return gripperSet(TAG_SPEED_LEFT, 4, (Gripper*)pGripper, (void*)buf, timeout, callBack);
}

int32_t __stdcall gripperSetPosition(GRIPPER_HANDLE pGripper, float left_position, float right_position, int32_t timeout, Callback_t callBack) {
	int16_t buf[2];

	buf[0] = (uint16_t)(left_position / 360.0f*4096.0f);
	buf[1] = (uint16_t)(right_position / 360.0f*4096.0f);

	return gripperSet(TAG_POSITION_LEFT, 4, (Gripper*)pGripper, (void*)buf, timeout, callBack);
}

int32_t __stdcall gripperSetOpenState(GRIPPER_HANDLE pGripper, uint16_t openstate, int32_t timeout, Callback_t callBack) { //us
	return gripperSet(TAG_OPEN_STA, 2, (Gripper*)pGripper, (void*)&openstate, timeout, callBack);
}

int32_t __stdcall gripperSetOpenAngle(GRIPPER_HANDLE pGripper, float openangle, int32_t timeout, Callback_t callBack) { //us
	uint16_t buf;

	if (openangle < 0.0f) return MR_ERROR_ILLDATA;

	buf = (uint16_t)(openangle / 2.0f / 360.0f*4096.0f);
	return gripperSet(TAG_OPEN_ANGLE, 2, (Gripper*)pGripper, (void*)&openangle, timeout, callBack);
}



