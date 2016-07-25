/***************************Copyright BestFu ***********************************
**  文    件：  Cube6.h
**  功    能：  <<逻辑层>> 魔方六面体逻辑层接口
**  编    译：  Keil uVision5 V5.12
**  版    本：  V1.1
**  编    写：  Seven
**  修    改：  Jay
**  创建日期：  2014.05.26
**  修改日期：  2015.10.30
**  说    明：  
**  V1.1
    >> 
*******************************************************************************/
#ifndef _CUBE_6_H_
#define _CUBE_6_H_

#include "MPU6050.h"

/*******************************数据结构定义**********************************/
typedef enum										/**> 魔方面号结构体 **/
{
	FACE_NULL,										/**> 无面 				**/
	FACE_ONE ,										/**> 面1 				**/
	FACE_TWO,										/**> 面2				**/
	FACE_THREE,										/**> 面3				**/
	FACE_FOURE,										/**> 面4				**/
	FACE_FIVE,										/**> 面5				**/
	FACE_SIX										/**> 面6				**/
}CubeFace_t,*pCubeFace_t;

typedef struct
{
	u16 ShockOnTime;								/**> 马达震动时间 		**/
	u16 ShockOffTime;								/**> 马达停止时间 		**/
	u8  ShockRepeatCnt;								/**> 震动重复次数 		**/
}ShockTime_t,pShockTime_t;

typedef struct
{
	CubeFace_t 	CurEnFace;							/**> 当前确认魔方面 	**/
	CubeFace_t 	LstEnFace;							/**> 上次确认魔方面 	**/	
	u8 	FirstChargeFlag;							/**> 魔方首次充电标志 	**/
	ShockTime_t ShockTime;						    /**> 马达震动时间结构体 **/
	u8 LowVoltageTips;								/**> 低电压提示标记     **/
#ifdef CHARGEDONETIP
	u8 	ChargeComplete;								/**> 魔方充电完成标志 	**/
#endif
	u8 PowerOnFlag;									/**> 上电标志 			**/
}CubeHandle_t,*pCubeHandle_t;

/******************************变量宏定义************************************/

#define ACCE_OFFSRT				     (25)			/**> 魔方误差线阈值 **/
#define CUBE_EEPROM_START_ADDR       (0x4000)		/**> 魔方的记录上次EEPROM地址 **/
/**********************************变量声明**********************************/

extern CubeHandle_t CubeHandle;       

/******************************函数声明***************************************/
extern void  Cube6_SearchFace(void);
extern void  Cube6_SaveEeprom(void);
extern void  Cube6_ReadEeprom(void);
extern void VibratingAlert(u16 OnTime,u16 OffTime,u8 RepeaptCnt);

#endif

/***************************Copyright BestFu **********************************/
