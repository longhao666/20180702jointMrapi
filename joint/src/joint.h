//  joint.h
//
//  ~~~~~~~~~~~~
//
//  Modular-Robot API
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Author : Lou Wei
//	Last change: 18.05.2018 Beijing
//
//  Language: ANSI-C
//  ------------------------------------------------------------------
//
//  Copyright (C) 2015-2018  Aubo Robotics, Beijing
//  more Info at http://www.aubo-robotics.cn 
//
#ifndef __JOINT_H__
#define __JOINT_H__
#include "mrapi.h"

// proctocol definition
// System infomation of joint driver board
//  
#define SYS_ID                0x01    //驱动器ID
#define SYS_MODEL_TYPE        0x02    //驱动器型号
#define SYS_FW_VERSION        0x03    //固件版本
#define SYS_ERROR             0x04    //错误代码
#define SYS_VOLTAGE           0x05    //系统电压
#define SYS_TEMP              0x06    //系统温度
#define SYS_REDU_RATIO        0x07    //模块减速比
//#define SYS_BAUDRATE_232      0x08    //232端口波特率
#define SYS_BAUDRATE_CAN      0x09    //CAN总线波特率
#define SYS_ENABLE_DRIVER     0x0A    //驱动器使能标志
#define SYS_ENABLE_ON_POWER   0x0B    //上电使能驱动器标志
#define SYS_SAVE_TO_FLASH     0x0C    //保存数据到Flash标志
#define SYS_IAP               0x0D    //下次上电进入bootloader
#define SYS_SET_ZERO_POS      0x0E    //将当前位置设置为零点标志
#define SYS_CLEAR_ERROR       0x0F    //清除错误标志

#define SYS_CURRENT_L         0x10    //当前电流低16位（mA）
#define SYS_CURRENT_H         0x11    //当前电流高16位（mA）
#define SYS_SPEED_L           0x12    //当前速度低16位（units/s）
#define SYS_SPEED_H           0x13    //当前速度高16位（units/s）
#define SYS_POSITION_L        0x14    //当前位置低16位（units）
#define SYS_POSITION_H        0x15    //当前位置高16位（units）
#define SYS_POTEN_VALUE       0x16    //数字电位器值
#define SYS_ZERO_POS_OFFSET_L 0x17    //零点位置偏移量低16位（units）
#define SYS_ZERO_POS_OFFSET_H 0x18    //零点位置偏移量高16位（units）
#define SYS_TORQUE_L          0x19		//当前转矩低16位（Nm, float型)**********************************2018.5.15
#define SYS_TORQUE_H          0x1A		//当前转矩高16位（Nm, float型)**********************************2018.5.15

// Motor infomation
#define MOT_RES               0x20    //电机内阻
#define MOT_INDUC             0x21    //电机电感
#define MOT_RATED_VOL         0x22    //电机额定电压
#define MOT_RATED_CUR         0x23    //电机额定电流
#define MOT_TORQ_COEFF        0x24		//转矩系数 mNm/A           **********************************2018.5.15
//#define MOT_HALL_VALUE        0x25    //当前霍尔状态
#define MOT_ST_DAT            0x26    //绝对编码器单圈数据
#define MOT_MT_DAT            0x27    //绝对编码器多圈数据
#define MOT_ENC_STA           0x28    //绝对编码器状态寄存器
#define BAT_VOLT              0x29    //编码器电池电压 *10mV
#define ACC_X                 0x2A    //加速度计x轴 *1000mg
#define ACC_Y                 0x2B    //加速度计y轴 *1000mg
#define ACC_Z                 0x2C    //加速度计z轴 *1000mg

// Control target infomation of joint driver board 
#define TAG_WORK_MODE         0x30    //工作模式，0-开环，1-电流模式，2-速度模式，3-位置模式
#define TAG_OPEN_PWM          0x31    //开环模式下占空比（0~100）
#define TAG_CURRENT_L         0x32    //目标电流低16位（mA）
#define TAG_CURRENT_H         0x33    //目标电流高16位（mA）
#define TAG_SPEED_L           0x34    //目标速度低16位（units/s）
#define TAG_SPEED_H           0x35    //目标速度高16位（units/s）
#define TAG_POSITION_L        0x36    //目标位置低16位（units）
#define TAG_POSITION_H        0x37    //目标位置高16位（units）
#define TAG_TORQUE_L		  0x36		//目标力矩低16位（Nm, float型）**********************************2018.5.15
#define TAG_TORQUE_H  		  0x37		//目标力矩高16位（Nm, float型）**********************************2018.5.15

// Control limitation infomation of joint driver board
#define LIT_MAX_CURRENT       0x40    //最大电流（mA）
#define LIT_MAX_SPEED         0x41    //最大速度（rpm）
#define LIT_MAX_ACC           0x42    //最大加速度（rpm/s）
#define LIT_MIN_POSITION_L    0x43    //最小位置低16位（units）
#define LIT_MIN_POSITION_H    0x44    //最小位置高16位（units）
#define LIT_MAX_POSITION_L    0x45    //最大位置低16位（units）
#define LIT_MAX_POSITION_H    0x46    //最大位置高16位（units）

// PID
#define SEV_PARAME_LOCKED     0x50    //三闭环参数锁定标志, 0-不锁定(自动切换), 1-低速（S）,2-中速（M）,3-高速（L）

#define S_CURRENT_P           0x51    //电流环P参数
#define S_CURRENT_I           0x52    //电流环I参数
#define S_CURRENT_D           0x53    //电流环D参数
#define S_SPEED_P             0x54    //速度环P参数
#define S_SPEED_I             0x55    //速度环I参数
#define S_SPEED_D             0x56    //速度环D参数
#define S_SPEED_DS            0x57    //速度P死区
#define S_POSITION_P          0x58    //位置环P参数
#define S_POSITION_I          0x59    //位置环I参数
#define S_POSITION_D          0x5A    //位置环D参数
#define S_POSITION_DS         0x5B    //位置P死区
#define S_CURRENT_FD          0x5C    //电流前馈
#define M_CURRENT_FD          0x5D    //电流前馈
#define L_CURRENT_FD          0x5E    //电流前馈

#define M_CURRENT_P           0x61    //电流环P参数
#define M_CURRENT_I           0x62    //电流环I参数
#define M_CURRENT_D           0x63    //电流环D参数
#define M_SPEED_P             0x64    //速度环P参数
#define M_SPEED_I             0x65    //速度环I参数
#define M_SPEED_D             0x66    //速度环D参数
#define M_SPEED_DS            0x67    //速度P死区
#define M_POSITION_P          0x68    //位置环P参数
#define M_POSITION_I          0x69    //位置环I参数
#define M_POSITION_D          0x6A    //位置环D参数
#define M_POSITION_DS         0x6B    //位置P死区

#define L_CURRENT_P           0x71    //电流环P参数
#define L_CURRENT_I           0x72    //电流环I参数
#define L_CURRENT_D           0x73    //电流环D参数
#define L_SPEED_P             0x74    //速度环P参数
#define L_SPEED_I             0x75    //速度环I参数
#define L_SPEED_D             0x76    //速度环D参数
#define L_SPEED_DS            0x77    //速度P死区
#define L_POSITION_P          0x78    //位置环P参数
#define L_POSITION_I          0x79    //位置环I参数
#define L_POSITION_D          0x7A    //位置环D参数
#define L_POSITION_DS         0x7B    //位置P死区

// 刹车控制命令
#define BRAKE_RELEASE_CMD     0x80    //刹车释放命令，0-保持制动，1-释放刹车
#define BRAKE_STATE           0x81    //刹车状态，0-保持制动，1-释放刹车

#define TC_COUL_COEFF         0x82    // ‰
#define TC_FRICRISE_COEFF     0x83    // 万分之一
#define TC_COUL_P             0x84
#define TC_COUL_N             0x85
#define TC_VISCOEFF_P         0x86
#define TC_VISCOEFF_N         0x87
#define TC_OSC_DEDUC          0x88   // ％
#define TC_OSC_FREQ           0x89   // 十分之一
#define TC_ZEROVEL_THR        0x8A   // 百分之一RPM
#define TC_CRITACC_THR        0x8B   // 百分之一RPM/s
#define TC_STARTUP_TW         0x8C   // 1/2K s
#define TC_STOP_TW            0x8D   // 1/2K s


// 示波器模块子索引地址定义
#define SCP_MASK              0x90    //记录对象标志MASK
#define SCP_REC_TIM           0x91    //记录时间间隔（ms）

#define SCP_TAGCUR_L          0x92    //目标电流数据集
#define SCP_TAGCUR_H          0x93    //目标电流数据集
#define SCP_MEACUR_L          0x94    //实际电流数据集
#define SCP_MEACUR_H          0x95    //实际电流数据集
#define SCP_TAGSPD_L          0x96    //目标速度数据集
#define SCP_TAGSPD_H          0x97    //目标速度数据集
#define SCP_MEASPD_L          0x98    //实际速度数据集
#define SCP_MEASPD_H          0x99    //实际速度数据集
#define SCP_TAGPOS_L          0x9A    //目标位置数据集
#define SCP_TAGPOS_H          0x9B    //目标位置数据集
#define SCP_MEAPOS_L          0x9C    //实际位置数据集
#define SCP_MEAPOS_H          0x9D    //实际位置数据集

typedef struct td_joint
{
	void* basicModule;
	uint16_t* jointId;
	uint16_t* jointType;
	uint16_t* jointRatio;
} Joint;

///For advanced users
#ifdef __cplusplus
extern "C" {
#define _DEF_ARG =0
#else
#define _DEF_ARG
#endif

/// <summary> Joint get. </summary>
/// <remarks> Get information from joint driver board. </remarks>
/// <param name="index">   	Zero-based index of the memory table. </param>
/// <param name="datLen">  	Length of the data to get. </param>
/// <param name="pJoint">  	If non-null, the joint handle. </param>
/// <param name="data">	   	[in] If non-null, the data to get. </param>
/// <param name="timeout"> 	The timeout in us. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGet(
	uint8_t index, 
	uint8_t datLen, 
	Joint* pJoint, 
	void* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set. </summary>
/// <remarks> Set configuration to joint. </remarks>
/// <param name="index">   	Zero-based index of the memory table. </param>
/// <param name="datLen">  	Length of the data to set. </param>
/// <param name="pJoint">  	If non-null, the joint handle. </param>
/// <param name="data">	   	[out] If non-null, the data to set. </param>
/// <param name="timeout"> 	The timeout in us. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSet(
	uint8_t index, 
	uint8_t datLen, 
	Joint* pJoint, 
	void* data, 
	int32_t timeout, 
	Callback_t callBack);

#ifdef __cplusplus
}
#endif
#endif

