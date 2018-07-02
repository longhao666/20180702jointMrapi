//  mrapi.h
//
//  ~~~~~~~~~~~~
//
//  Modular-Robot API
//
//  ~~~~~~~~~~~~
//
//  ------------------------------------------------------------------
//  Author : Lou Wei
//	Last change: 2018.07.02 Beijing
//
//  Language: ANSI-C
//  ------------------------------------------------------------------
//
//  Copyright (C) 2015-2018  Aubo Robotics, Beijing
//  more Info at http://www.aubo-robotics.cn 
//
#ifndef _MRAPI_H_
#define _MRAPI_H_
#include <stdint.h>

#ifndef _WINDOWS
#define __stdcall
#endif

////////////////////////////////////////////////////////////
// Value definitions
////////////////////////////////////////////////////////////

// Maximum CAN device number supported by this API
// 
#define MAX_CAN_DEVICES 4 

// Maximum Joints number that can be connected to all the devices
// 
#define MAX_JOINTS 20 

// Maximum Grippers number that can be connected to all the devices
// 
#define MAX_GRIPPERS 3     

// CAN devices id
// 
#define MASTER(x) x     

// Represent the joint types supported by this API
// 
#define MODEL_TYPE_M14        0x010		// 
#define MODEL_TYPE_M17        0x020		//
#define MODEL_TYPE_M17V2      0x021		//
#define MODEL_TYPE_M20        0x030		//
#define MODEL_TYPE_M20V2      0x031		//
#define MODEL_TYPE_M20V3      0x032		//
#define MODEL_TYPE_MRG2       0x080		//

// Represent the work modes supported by Joint
//
typedef enum {
	joint_open = 0,         // Open mode
	joint_current = 1,      // Current/Torque servo mode
	joint_speed = 2,        // Speed servo mode
	joint_position = 3,     // Position servo mode
	joint_cyclesync = 4,    // PVT mode
}jointMode_t;

// Represent the work modes supported by Joint
//
typedef enum {
	gripper_none = 0,       // Open mode
	gripper_openclose = 1,  // Current/Torque servo mode
	gripper_position = 2,   // Speed servo mode
	gripper_servo = 3,      // Position servo mode
}gripperMode_t;

// Represent the MRAPI error and status codes
//
#define MR_ERROR_OK                0x00000U  // No error
#define MR_ERROR_TIMEOUT           0x00001U  // Read/Write timeout error
#define MR_ERROR_BUSY              0x00002U  // Read/Write in progress
#define MR_ERROR_ACK0              0x00004U  // Write acknowlogy 0
#define MR_ERROR_ACK1              0x00008U  // Write acknowlogy 1
#define MR_ERROR_QXMTFULL          0x00010U  // Transmit queue is full
#define MR_ERROR_QXMTEMPTY         0x00020U  // Transmit queue is empty
#define MR_ERROR_ILLDATA           0x00040U  // Invalid data, function, or action
#define MR_ERROR_OFFLINE           0x00080U  // Invalid data, function, or action

// Represent the Error bit mask
// 
#define ERROR_MASK_OVER_CURRENT   0x0001    // Current overflow
#define ERROR_MASK_OVER_VOLTAGE   0x0002    // Voltage overflow
#define ERROR_MASK_UNDER_VOLTAGE  0x0004    // Voltage underflow
#define ERROR_MASK_OVER_TEMP      0x0008    // Temperature too high
#define ERROR_MASK_BATTERY        0x0010    // Battery voltage low
#define ERROR_MASK_ENCODER        0x0020    // Encoder Error
#define ERROR_MASK_POTEN          0x0040    // Poten Error
#define ERROR_MASK_CURRENT_INIT   0x0080    // Current calibration Error
#define ERROR_MASK_FUSE           0x0100    // Fuse off
#define ERROR_MASK_RX28			  0x0200	// rx28 error

// A macro that defines CAN baudrate
// 
#define BAUD_CAN_250K			0		//250K
#define BAUD_CAN_500K			1		//500K
#define BAUD_CAN_1M		        2		//1M

// Represent the Scope bit mask
// 
#define SCP_MASK_TAGCUR				0x0001		// Bit of monitoring target current
#define SCP_MASK_MEACUR				0x0002		// Bit of monitoring actual current
#define SCP_MASK_TAGSPD				0x0004		// Bit of monitoring target speed
#define SCP_MASK_MEASPD				0x0008		// Bit of monitoring actual speed
#define SCP_MASK_TAGPOS				0x0010		// Bit of monitoring target position
#define SCP_MASK_MEAPOS				0x0020		// Bit of monitoring actual position

// Gripper update servo information mask
// 
#define UPDATE_LEFT_SPD				0x01
#define UPDATE_LEFT_POS				0x02
#define UPDATE_LEFT_TMP				0x04
#define UPDATE_LEFT_TRQ				0x08
#define UPDATE_RIGHT_SPD			0x10
#define UPDATE_RIGHT_POS			0x20
#define UPDATE_RIGHT_TMP			0x40
#define UPDATE_RIGHT_TRQ			0x80


#define isJointType(t) (t==MODEL_TYPE_M14)||(t==MODEL_TYPE_M17)||(t==MODEL_TYPE_M17V2)||(t==MODEL_TYPE_M20)||(t==MODEL_TYPE_M20V2) \
						||(t==MODEL_TYPE_M20V3)
#define isGripperType(t) (t==MODEL_TYPE_MRG2)
#define isJointMode(t) ((t==joint_open)||(t==joint_current)||(t==joint_speed)||(t==joint_position)||(t==joint_cyclesync))
#define isGripperMode(t) ((t==gripper_none)||(t==gripper_openclose)||(t==gripper_position)||(t==gripper_servo))

////////////////////////////////////////////////////////////
// Structure definitions
////////////////////////////////////////////////////////////

/// <summary> Defines an alias representing handle of the joint. </summary>
/// <remarks> Represents a Joint hardware. </remarks>
typedef void* JOINT_HANDLE;

/// <summary> Defines an alias representing handle of the gripper. </summary>
/// <remarks> Represents a Gripper hardware. </remarks>
typedef void* GRIPPER_HANDLE;

/// <summary> Defines an alias representing the set/get callback function. </summary>
/// <remarks> Provide callback function templet. </remarks>
typedef int32_t(*Callback_t)(uint16_t id, uint16_t index, void* args);

#ifdef __cplusplus
extern "C" {
#define _DEF_ARG =0
#else
#define _DEF_ARG
#endif

////////////////////////////////////////////////////////////
// Modular-Robot API function declarations
////////////////////////////////////////////////////////////

/// <summary> Starts a CAN device as a master. </summary>
/// <remarks> PEAK CAN devices or Ethernet CAN devices depend on the make option. </remarks>
/// <param name="busname"> 	The busname. </param>
/// <param name="masterId">	Identifier for the master. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall startMaster(
	const char* busname,
	uint8_t masterId);

/// <summary> Stops a master by its identifier. </summary>
/// <remarks> Close CAN devices. </remarks>
/// <param name="masterId">	Identifier for the master. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall stopMaster(
	uint8_t masterId);

/// <summary> Wait for a master to be terminated. </summary>
/// <remarks> join thread of CAN reading. </remarks>
/// <param name="masterId">	Identifier for the master. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall joinMaster(
	uint8_t masterId);

/// <summary> Joint up. </summary>
/// <remarks> Construct Joint and put it in Joint stack. The Joint should be connected to the
/// assigned master. IF joint is offline(manually removed by jointDown or disconnect from the
/// BUS). </remarks>
/// <param name="jointId"> 	Identifier for the joint. </param>
/// <param name="masterId">	Identifier for the master. </param>
/// <returns> A JOINT_HANDLE. </returns>
JOINT_HANDLE __stdcall jointUp(
	uint16_t jointId, 
	uint8_t masterId); 

/// <summary> Joint down. </summary>
/// <remarks> Destruct joint and remove it from joint stack. </remarks>
/// <param name="pJoint">	The joint handle. </param>
/// <returns> A MRAPI error code. </returns>
int32_t      __stdcall jointDown(
	JOINT_HANDLE *ppJoint); 

/// <summary> Joint select. </summary>
/// <remarks> Find joint by it's ID. </remarks>
/// <param name="id">	The joint identifier. </param>
/// <returns> A JOINT_HANDLE. </returns>
JOINT_HANDLE __stdcall jointSelect(
	uint16_t id);  

/// <summary> Joint push. </summary>
/// <remarks> Set Joint output target position, speed and current to a joint. This API works only
/// in work mode of joint_cyclesync. </remarks>
/// <param name="h">	   	The joint handle. </param>
/// <param name="pos">	   	[out] If non-null, Target position in degree. </param>
/// <param name="speed">   	[out] If non-null, Target speed in degree per second. </param>
/// <param name="_DEF_ARG">	[out] If non-null, Target current in mA. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointPush(
	JOINT_HANDLE h, 
	float pos,
	float speed,
	float current _DEF_ARG);

/// <summary> Joint poll. </summary>
/// <remarks> Poll Joint output real position, speed and current of a joint from ram. </remarks>
/// <param name="h">	  	The joint handle. </param>
/// <param name="pos">	  	[in] If non-null, the position in degree. </param>
/// <param name="speed">  	[in] If non-null, the speed in degree per second. </param>
/// <param name="current">	[in] If non-null, the current in mA. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointPoll(
	JOINT_HANDLE h, 
	float* pos, 
	float* speed,
	float* current);

/// <summary> Joint poll scope. </summary>
/// <remarks> Poll Joint output real position, speed and current of a joint from scope from ram,
/// jointSetScpMask and jointSetScpInterval should be called before. </remarks>
/// <param name="h">	  	The joint handle. </param>
/// <param name="pos">	  	[in] If non-null, the position in degree. </param>
/// <param name="speed">  	[in] If non-null, the speed in degree per second. </param>
/// <param name="current">	[in] If non-null, the current in mA. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointPollScope(
	JOINT_HANDLE h, 
	float* pos,
	float* speed,
	float* current);

/// <summary> Joint get identifier. </summary>
/// <remarks> Get joint ID by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the id. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetId(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get module type. </summary>
/// <remarks> Get joint module type by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the module type, refer MODEL_TYPE_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second.  If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetType(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get error code. </summary>
/// <remarks> Get joint error  by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the error code, refer ERROR_MASK_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetError(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get reduce ratio. </summary>
/// <remarks> Get joint reduce ratio. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the reduce ratio. </param>
/// <param name="timeout"> 	The timeout in micro second.If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetRatio(JOINT_HANDLE pJoint, uint16_t* data, int32_t timeout, Callback_t callBack);

/// <summary> Joint get bus voltage. </summary>
/// <remarks> Get joint bus voltage by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the bus voltage in Volt. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetVoltage(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get temperature. </summary>
/// <remarks> Get joint teperature by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the joint temperature in 0.01 Celsius degree. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetTemp(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get CAN baudrate. </summary>
/// <remarks> Get joint communication baudrate by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the CAN baudrate, refer BAUD_CAN_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetBaudrate(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get current. </summary>
/// <remarks> Get joint real current by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the current in milli Ampere. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetCurrent(
	JOINT_HANDLE pJoint, 
	uint32_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get speed. </summary>
/// <remarks> Get joint speed by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the motor speed without reduce ratio involved. 1
/// 						unit = 360/65536 degree/s. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetSpeed(
	JOINT_HANDLE pJoint, 
	uint32_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get output position. </summary>
/// <remarks> Get joint position by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the joint out put position. 1 unit = 360/65536
/// 						degree. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetPosition(
	JOINT_HANDLE pJoint, 
	uint32_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get work mode. </summary>
/// <remarks> Get joint work mode by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the joint work mode, refer jointMode_t enum
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetMode(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get motor maximum speed. </summary>
/// <remarks> Get motor maximum speed by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the motor output maximum speed in RPM without speed
/// 						reducing. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetMaxSpeed(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get motor maximum acceleration. </summary>
/// <remarks> Get motor maxium acceleration by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the motor output maximum acceleration in rpm/s
/// 						without speed reducing. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetMaxAcceleration(
	JOINT_HANDLE pJoint, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get position limit. </summary>
/// <remarks> Get joint position limit by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="data">	   	[in] If non-null, the joint output position limit, data[0] represents
/// 						lower limit and data[1] represents upper limit, 1 unit = 360/65536
/// 						degree. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetPositionLimit(
	JOINT_HANDLE pJoint,
	int32_t* data,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get current loop p gain. </summary>
/// <remarks> Get joint p gain of current loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	[in] If non-null, the joint p gain of current loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetCurrP(
	JOINT_HANDLE pJoint,
	uint16_t* pValue, 
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint get current loop i gain. </summary>
/// <remarks> Gei joint i gain of current loop by its handle. </remarks>
/// <param name="pJoint">  	The joint. </param>
/// <param name="iValue">  	[in] If non-null, joint i gain of current. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetCurrI(
	JOINT_HANDLE pJoint, 
	uint16_t* iValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get speed loop p gain. </summary>
/// <remarks> Get joint p gain of speed loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	[in] If non-null, the p gain of speed loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetSpeedP(
	JOINT_HANDLE pJoint,
	uint16_t* pValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get speed loop i gain. </summary>
/// <remarks> Get joint i gain of speed loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="iValue">  	[in] If non-null, joint i gain of speed loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetSpeedI(
	JOINT_HANDLE pJoint, 
	uint16_t* iValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get position loop p gain. </summary>
/// <remarks> Get joint p gain of position loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	[in] If non-null, the joint p gain of position loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetPositionP(
	JOINT_HANDLE pJoint,
	uint16_t* pValue,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint get position loop dead area. </summary>
/// <remarks> Get joint dead area of position loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="dsValue"> 	[in] If non-null, the joint dead area of position loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointGetPositionDs(
	JOINT_HANDLE pJoint, 
	uint16_t* dsValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set identifier. </summary>
/// <remarks> Set joint id by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="id">	   	The identifier. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetID(
	JOINT_HANDLE pJoint,
	uint16_t id, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set baudrate. </summary>
/// <remarks> Set joint communication baudrate by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="baud">	   	The CAN baudrate, refer BAUD_CAN_XXX macro definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetBaudrate(
	JOINT_HANDLE pJoint, 
	uint16_t baud, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set enable. </summary>
/// <remarks> Set joint enable by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="isEnable">	1 - enable, 0 - disable. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetEnable(
	JOINT_HANDLE pJoint, 
	uint16_t isEnable,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set power on status. </summary>
/// <remarks> Set joint status upon power on by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="isEnable">	1 - enable the joint upon poweron, 0 - disable the joint upon
/// 						poweron. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetPowerOnStatus(
	JOINT_HANDLE pJoint,
	uint16_t isEnable,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set save to flash. </summary>
/// <remarks> Save changes to flash by its handle. </remarks>
/// <param name="pJoint">  	The joint. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetSave2Flash(
	JOINT_HANDLE pJoint, 
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint set zero position. </summary>
/// <remarks> Set joint present position to zero position by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetZero(
	JOINT_HANDLE pJoint,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set clear error. </summary>
/// <remarks> Clear error of joint by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetClearError(
	JOINT_HANDLE pJoint, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set mode. </summary>
/// <remarks> Set joint work mode by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="mode">	   	The joint work mode, refer jointMode_t enum definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetMode(
	JOINT_HANDLE pJoint, 
	jointMode_t mode,
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint set target current. </summary>
/// <remarks> Set joint target current in work mode of joint_current, set the joint work mode to
/// joint_current by jointSetMode function. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="current"> 	The current unit is Amps. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetCurrent(
	JOINT_HANDLE pJoint, 
	float current, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set target speed. </summary>
/// <remarks> Set joint target speed by its handle, set the joint work mode to joint_current by
/// jointSetMode function before this function is called. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="speed">   	The speed unit is degree/s. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetSpeed(
	JOINT_HANDLE pJoint, 
	float speed,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set target position. </summary>
/// <remarks> Set joint target position by its handle, set the joint work mode to joint_position by
/// jointSetMode function before this function is called. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="position">	The position unit is degree. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetPosition(
	JOINT_HANDLE pJoint,
	float position,
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint set maximum speed. </summary>
/// <remarks> Set joint maximum speed by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="maxspeed">	The maxspeed of the selected joint, the unit is degree per second. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetMaxSpeed(
	JOINT_HANDLE pJoint, 
    float maxspeed,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set maximum acceleration. </summary>
/// <remarks> Set maximum acceleration of joint by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="maxacc">  	The maxacc, the unit is degree per second^2. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetMaxAcceleration(
	JOINT_HANDLE pJoint,
    float maxacc,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set position limit. </summary>
/// <remarks> Set joint position limit by its handle, if position_min equal to position_max, there
/// will be no limitation to Joint. The limitation will only effect in joint_position mode. </remarks>
/// <param name="pJoint">	   	The joint handle. </param>
/// <param name="position_min">	The position minimum in degree. </param>
/// <param name="position_max">	The position maximum in degree. </param>
/// <param name="timeout">	   	The timeout in micro second. If timeout equal to 0, data is
/// 							fetched from ram immediately and no communication will happen. If
/// 							timeout equal to 0xffff(-1), function will return immediately and
/// 							callback function will be called onece acknowlege received. </param>
/// <param name="callBack">	   	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetPositionLimit(
	JOINT_HANDLE pJoint,
	float position_min,
	float position_max,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set current loop p gain. </summary>
/// <remarks> Set joint p gain of current loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	The joint p gain of current loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetCurrP(
	JOINT_HANDLE pJoint, 
	uint16_t pValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set current loop i gain. </summary>
/// <remarks> Set joint i gain of current loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="iValue">  	The joint i gain of current loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetCurrI(
	JOINT_HANDLE pJoint, 
	uint16_t iValue, 
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint set speed loop p gain. </summary>
/// <remarks> Set joint p gain of speed loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	The joint p gain of speed loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetSpeedP(
	JOINT_HANDLE pJoint,
	uint16_t pValue,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set speed loop i gain. </summary>
/// <remarks> Set joint i gain of speed loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="iValue">  	The joint i gain of speed loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetSpeedI(
	JOINT_HANDLE pJoint, 
	uint16_t iValue, 
	int32_t timeout,
	Callback_t callBack);

/// <summary> Joint set position loop p gain. </summary>
/// <remarks> Set joint p gain of position loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="pValue">  	The joint p gain of position loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetPositionP(
	JOINT_HANDLE pJoint, 
	uint16_t pValue, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set position loop dead area. </summary>
/// <remarks> Set joint dead area of position loop by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="dsValue"> 	The joint dead area of position loop. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetPositionDs(
	JOINT_HANDLE pJoint,
	uint16_t dsValue,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set scope mask. </summary>
/// <remarks> Set scope mask of joint by its handle. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="mask">	   	The scope mask, refer SCP_MASK_XXX macro definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetScpMask(
	JOINT_HANDLE pJoint, 
	uint16_t mask, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set scope interval. </summary>
/// <remarks> Set scope interval of joint by its handle. Control the frequency of joint information
/// upload. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="interval">	The interval, ms. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetScpInterval(
	JOINT_HANDLE pJoint, 
	uint16_t interval, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Joint set bootloader. </summary>
/// <remarks> Set bootloader trigger. If mask=1, joint will go into bootloader mode after next
/// power on. If mask=2, joint go into booloader mode immediately. </remarks>
/// <param name="pJoint">  	The joint handle. </param>
/// <param name="mask">	   	The bootload option. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The callback function. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall jointSetBootloader(
	JOINT_HANDLE pJoint, 
	uint16_t mask,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper select. </summary>
/// <remarks> Find Gripper by it's ID. </remarks>
/// <param name="id">	The identifier. </param>
/// <returns> A Gripper handle. </returns>
GRIPPER_HANDLE __stdcall gripperSelect(
	uint16_t id);

/// <summary> Gripper push. </summary>
/// <remarks> Set target position of both fingers to a Gripper. This API works only in work mode of
/// gripper_servo. </remarks>
/// <param name="h">			The Gripper handle. </param>
/// <param name="left_pos"> 	The left position. </param>
/// <param name="right_pos">	The right position. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperPush(
	GRIPPER_HANDLE h, 
	float left_pos, 
	float right_pos);

/// <summary> Gripper poll. </summary>
/// <remarks> Get target position and torque of both fingers to a Gripper. </remarks>
/// <param name="h">		   	The Gripper handle. </param>
/// <param name="left_pos">	   	[in] If non-null, the left position. </param>
/// <param name="right_pos">   	[in] If non-null, the right position. </param>
/// <param name="left_torque"> 	[in] If non-null, the left torque. </param>
/// <param name="right_torque">	[in] If non-null, the right torque. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperPoll(
	GRIPPER_HANDLE h, 
	float* left_pos, 
	float* right_pos, 
	float* left_torque, 
	float* right_torque);

/// <summary> Gripper up. </summary>
/// <remarks> Construct Gripper and put it in Gripper stack. The Gripper should be connected to the
/// assigned master. </remarks>
/// <param name="gripperId">	Identifier for the gripper. </param>
/// <param name="masterId"> 	Identifier for the master. </param>
/// <returns> A GRIPPER_HANDLE. </returns>
GRIPPER_HANDLE __stdcall gripperUp(
	uint16_t gripperId, 
	uint8_t masterId);

/// <summary> Gripper down. </summary>
/// <remarks> Destruct Gripper and remove it from Gripper stack. </remarks>
/// <param name="h">	The Gripper handle. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperDown(
	GRIPPER_HANDLE h);

/// <summary> Gripper get voltage. </summary>
/// <remarks> Get Gripper bus voltage by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the data. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetVoltage(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get temperature. </summary>
/// <remarks> Get gripper teperature by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the data. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetTemp(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get baudrate. </summary>
/// <remarks> Get gripper communication baudrate by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the CAN baudrate, refer BAUD_CAN_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetBaudrate(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get position. </summary>
/// <remarks> Read position of both fingers. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, data[0] - position of left finger, data[1] -
/// 						position of right finger. 1 unit = 360/4069 degree. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetPosition(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get speed. </summary>
/// <remarks> Get Gripper speed of both fingers by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, data[0] - speed of left finger, data[1] - speed of
/// 						right finger. 1 unit = 0.229 RPM. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetSpeed(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get torque. </summary>
/// <remarks> Get Gripper Torque of both fingers by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, data[0] - Torque of left finger, data[1] - Torque
/// 						of right finger. 1 unit = 0.1% of maximum torque, range from -1000 to
/// 						1000. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetTorque(
	GRIPPER_HANDLE pGripper, 
	int16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get work mode. </summary>
/// <remarks> Get Gripper work mode by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the gripper work mode, refer gripperMode_t enum
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetMode(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get maximum speed. </summary>
/// <remarks> Get Gripper maximum speed of both fingers by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, data[0] - maximum speed of left finger, data[1] -
/// 						maximum speed of right finger. 1 unit = 0.229 RPM. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetMaxSpeed(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set enable. </summary>
/// <remarks> Set gripper enable torque output by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="isEnable">	1 - enable, 0 - disable. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetEnable(
	GRIPPER_HANDLE pGripper, 
	uint16_t isEnable, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set save to flash. </summary>
/// <remarks> Save changes to flash by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetSave2Flash(
	GRIPPER_HANDLE pGripper, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set zero. </summary>
/// <remarks> Set gripper present position to zero position by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetZero(
	GRIPPER_HANDLE pGripper, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set clear error. </summary>
/// <remarks> Clear error of gripper by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetClearError(
	GRIPPER_HANDLE pGripper, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set work mode. </summary>
/// <remarks> Set gripper work mode by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="mode">	   	The gripper work mode, refer gripperMode_t enum definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetMode(
	GRIPPER_HANDLE pGripper, 
	gripperMode_t mode, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get error code. </summary>
/// <remarks> Get Gripper error  by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the error code, refer ERROR_MASK_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetError(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get module type. </summary>
/// <remarks> Get gripper module type by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the gripper module, refer jointMode_t enum
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetType(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set CAN baudrate. </summary>
/// <remarks> Set gripper communication baudrate by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="baud">	   	The CAN baudrate, refer BAUD_CAN_XXX macro definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetBaudrate(
	GRIPPER_HANDLE pGripper, 
	uint16_t baud, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper get identifier. </summary>
/// <remarks> Get Gripper ID by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="data">	   	[in] If non-null, the identifier. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperGetId(
	GRIPPER_HANDLE pGripper, 
	uint16_t* data, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set identifier. </summary>
/// <remarks> Set gripper's ID by its handle. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="id">	   	The identifier. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetID(
	GRIPPER_HANDLE pGripper, 
	uint16_t id, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set speed. </summary>
/// <remarks> Set gripper moving speed of each finger. </remarks>
/// <param name="pGripper">   	The Gripper handle. </param>
/// <param name="speed_left"> 	The speed left in degree/s. </param>
/// <param name="speed_right">	The speed right in degree/s. </param>
/// <param name="timeout">	  	The timeout in micro second. If timeout equal to 0, data is
/// 							fetched from ram immediately and no communication will happen. If
/// 							timeout equal to 0xffff(-1), function will return immediately and
/// 							callback function will be called onece acknowlege received. </param>
/// <param name="callBack">   	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetSpeed(
	GRIPPER_HANDLE pGripper, 
	float speed_left,
	float speed_right,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set target positions of both fingers. </summary>
/// <remarks> Set the position of fingers in gripper_position mode. The work mode should be set to
/// gripper_position firstly. </remarks>
/// <param name="pGripper">		 	The Gripper handle. </param>
/// <param name="left_position"> 	The left position in degree. </param>
/// <param name="right_position">	The right position in degree. </param>
/// <param name="timeout">		 	The timeout in micro second. If timeout equal to 0, data is
/// 								fetched from ram immediately and no communication will
/// 								happen. If timeout equal to 0xffff(-1), function will return
/// 								immediately and callback function will be called onece
/// 								acknowlege received. </param>
/// <param name="callBack">		 	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetPosition(
	GRIPPER_HANDLE pGripper, 
	float left_position, 
	float right_position,
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set update. </summary>
/// <remarks> Indicate gripper to update related information, called before gripperGetXXX. </remarks>
/// <param name="pGripper">	The Gripper handle. </param>
/// <param name="mask">	   	The mask of information to be update, refer UPDATE_XXX_XXX macro
/// 						definition. </param>
/// <param name="timeout"> 	The timeout in micro second. If timeout equal to 0, data is fetched
/// 						from ram immediately and no communication will happen. If timeout
/// 						equal to 0xffff(-1), function will return immediately and callback
/// 						function will be called onece acknowlege received. </param>
/// <param name="callBack">	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetUpdate(
	GRIPPER_HANDLE pGripper, 
	uint16_t mask, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set open state. </summary>
/// <remarks> Set gripper finger open or close in gripper_openclose work mode. </remarks>
/// <param name="pGripper"> 	The Gripper handle. </param>
/// <param name="openstate">	The open state, 0 - close, 1 - open. </param>
/// <param name="timeout">  	The timeout in micro second. If timeout equal to 0, data is
/// 							fetched from ram immediately and no communication will happen. If
/// 							timeout equal to 0xffff(-1), function will return immediately and
/// 							callback function will be called onece acknowlege received. </param>
/// <param name="callBack"> 	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetOpenState(
	GRIPPER_HANDLE pGripper, 
	uint16_t openstate, 
	int32_t timeout, 
	Callback_t callBack);

/// <summary> Gripper set open angle. </summary>
/// <remarks> Set gripper finger open angle in gripper_openclose work mode. </remarks>
/// <param name="pGripper"> 	The Gripper handle. </param>
/// <param name="openangle">	The open angle indegree. </param>
/// <param name="timeout">  	The timeout in micro second. If timeout equal to 0, data is
/// 							fetched from ram immediately and no communication will happen. If
/// 							timeout equal to 0xffff(-1), function will return immediately and
/// 							callback function will be called onece acknowlege received. </param>
/// <param name="callBack"> 	The call back. </param>
/// <returns> A MRAPI error code. </returns>
int32_t __stdcall gripperSetOpenAngle(
	GRIPPER_HANDLE pGripper, 
	float openangle, 
	int32_t timeout, 
	Callback_t callBack);

#ifdef __cplusplus
}

#endif
#endif

