/***************************Copyright BestFu ***********************************
**  文    件：  Cube6.c
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
#include "Cube6.h"
#include "ShakeMotor.h"
#include "LowPower.h"
#include "UnitCfg.h"
#include "Thread.h" 
#include "GPIO.h"
#include "BatteryExeAttr.h"
#include "stm32l1xx.h"
#include "usart.h"
#include "Battery.h"

static s8 UpFaceTab[6][3]=   //六面体匹配表
{
    {   0,   0, 100},   //1
    { 100,   0,   0},   //5
    {   0,-100,   0},   //4
    {   0, 100,   0},   //3  
    {-100,   0,   0},   //2
    {   0,   0,-100}    //6
};

static u32 ChargeFlag = 0;		 						/**> 充电标志暂存的数 		**/
CubeHandle_t CubeHandle = {FACE_NULL,FACE_NULL,0};		/**> 定义魔方处理函数结构体 **/
static CubeFace_t FindCurrentFace(void);
static void ChargeComplete(void);
static u8 ChargeCheck(u32* Value,u8 checkvalue,u32 Accuracy);
static void ChargeComplete1(void);
static void ChargeCompleteDlr(void);

  
/*******************************************************************************
**函    数： void Cube6_SearchFace(void)
**功    能： 寻找及确认面值并处理
**参    数： void
**返    回： NULL
*******************************************************************************/
void Cube6_SearchFace(void)
{
	CubeFace_t CubeFaceTemp   = FACE_NULL;
	
    if(((GPIO_TypeDef*)(GPIO_BASE + (MOTOR_PORT<<10)))->ODR & MOTOR_PIN)  return ;     		/**> 震动保护 			*/
								
	if(ChargeCheck(&ChargeFlag,(u8)((0 == (GPIOA->IDR&(1U<<11)))?(1u):(0u)),0xfffff))   	/**> 若充电 			*/
	{
		if(!CubeHandle.FirstChargeFlag)                 									/**> 判断是否首次充电 	*/
		{
			CubeHandle.FirstChargeFlag = 1;
			VibratingAlert(120,240,1);
		}
		return ;
	}
	else
	{
		if(0 == (GPIOA->IDR&(1U<<11))) return;
		CubeHandle.FirstChargeFlag = 0;
	}
	CubeFaceTemp = FindCurrentFace();
	if(FACE_NULL != CubeFaceTemp)
	{
		CubeHandle.CurEnFace = CubeFaceTemp;
		if(CubeHandle.LstEnFace != CubeHandle.CurEnFace)
		{
			CubeHandle.LstEnFace   = CubeHandle.CurEnFace; 
			CubeHandle.PowerOnFlag = 0x00;	
			StandbyCountReset();			
			PropEventFifo(1, 0x01,SRCEVENT,CubeHandle.CurEnFace);

			if(1 == CubeHandle.LowVoltageTips)
			{
				VibratingAlert(60,240,2);
			}
			else
			{
				VibratingAlert(80,240,1);
			}
			Upload(); 
		}
		else
		{
			if(1 == CubeHandle.PowerOnFlag)
			{
				CubeHandle.PowerOnFlag = 0x00;
				StandbyCountReset();
				if(1 == CubeHandle.LowVoltageTips)
				{
					VibratingAlert(80,240,2);
				}
				else
				{
					VibratingAlert(100,240,1);
				}
				Upload(); 
			}
		}
	}
}

/*******************************************************************************
**函    数： static CubeFace_t FindCurrentFace(void)
**功    能： 查找当前面值
**参    数： void
**返    回： 当前面值
*******************************************************************************/
static CubeFace_t FindCurrentFace(void)
{
	u8 i;
	short int CurTmp[3] = {0};
    static CubeFace_t CubeFaceBackup = FACE_NULL;
	static u8 DebounceTimes = 0;
	
	MPU6050_ReadAcce(CurTmp);             				/**> 读取加速度 **/ 
    for(i = 0;i < 6;i++)
    {
		if( (CurTmp[0] >=UpFaceTab[i][0]-ACCE_OFFSRT)&& \
			(CurTmp[0] < UpFaceTab[i][0]+ACCE_OFFSRT)&& \
			(CurTmp[1] >=UpFaceTab[i][1]-ACCE_OFFSRT)&& \
			(CurTmp[1] < UpFaceTab[i][1]+ACCE_OFFSRT)&& \
			(CurTmp[2] >=UpFaceTab[i][2]-ACCE_OFFSRT)&& \
			(CurTmp[2] < UpFaceTab[i][2]+ACCE_OFFSRT))
		{          
			if(CubeFaceBackup == (CubeFace_t)(i + 1))
			{
				if(DebounceTimes++ > 4)       			/**> 消抖处理 **/
				{
					DebounceTimes = 0;
					return (CubeFaceBackup);
				}
			}
			else
			{
				CubeFaceBackup = (CubeFace_t)(i + 1);
				DebounceTimes = 0;
			}
		}			
    }
	return (FACE_NULL);
}

/*******************************************************************************
**函    数： Cube6_SaveEeprom
**功    能： 保存最后面值
**参    数： void
**返    回： void
*******************************************************************************/
void  Cube6_SaveEeprom(void)
{
    EEPROM_Write(CUBE_EEPROM_START_ADDR, 1, (u8*)(&CubeHandle.LstEnFace));
}

/*******************************************************************************
**函    数： Cube6_ReadEeprom
**功    能： 读取上次保存的面值
**参    数： void
**返    回： 匹配面(1-6)      0xFF   未匹配
*******************************************************************************/
void  Cube6_ReadEeprom(void)
{
    EEPROM_Read(CUBE_EEPROM_START_ADDR, 1, (u8*)(&CubeHandle.LstEnFace));
    if(CubeHandle.LstEnFace > 6)
    {
        CubeHandle.LstEnFace = FACE_NULL;
    }
}

/*******************************************************************************
**函    数： static void ChargeComplete1(void)
**功    能： 充电完成处理
**参    数： void
**返    回： 无
*******************************************************************************/
static void ChargeComplete1(void)
{
	ShakeMotor_Stop();
	CubeHandle.ShockTime.ShockRepeatCnt--;
	if(!CubeHandle.ShockTime.ShockRepeatCnt)
	{
		ChargeCompleteDlr();
		return;
	}
	Thread_Login(ONCEDELAY, 0 ,CubeHandle.ShockTime.ShockOffTime, &ChargeComplete);
}

/*******************************************************************************
**函    数： static void ChargeCompleteDlr(void)
**功    能： 充电完成处理线程清除函数
**参    数： void
**返    回： 无
*******************************************************************************/
static void ChargeCompleteDlr(void)
{
	Thread_Logout(ChargeComplete);
	Thread_Logout(ChargeComplete1);
}

/*******************************************************************************
**函    数： static void ChargeComplete(void)
**功    能： 充电完成处理
**参    数： void
**返    回： 无
*******************************************************************************/
static void ChargeComplete(void)
{
	ShakeMotor_Start();
	Thread_Login(ONCEDELAY, 0 ,CubeHandle.ShockTime.ShockOnTime, &ChargeComplete1);
}

/*******************************************************************************
**函    数： static u8 ChargeCheck(u32* Value,u8 checkvalue,u32 Accuracy)
**功    能： 充电检测
**参    数： Value：检测标记指针，checkvalue：待检测的变量,Accuracy:精度
**返    回： 无
*******************************************************************************/
static u8 ChargeCheck(u32* Value,u8 checkvalue,u32 Accuracy)
{
	*Value = (*Value<<1)|(checkvalue);
	if((*Value&(Accuracy)) == Accuracy)
	{
		return (1u);
	}
	return (0u);
}

/*******************************************************************************
**函    数： __inline void VibratingAlert(u16 OnTime,u16 OffTime,u8 RepeaptCnt)
**功    能： 震动提示函数
**参    数： OnTime：开时间，OffTime：关时间,RepeaptCnt:重复次数
**返    回： 无
*******************************************************************************/
__inline void VibratingAlert(u16 OnTime,u16 OffTime,u8 RepeaptCnt)
{
	ChargeCompleteDlr();
	CubeHandle.ShockTime.ShockOnTime   = OnTime;
	CubeHandle.ShockTime.ShockOffTime  = OffTime;
	CubeHandle.ShockTime.ShockRepeatCnt= RepeaptCnt;
	ChargeComplete();
}

/***************************Copyright BestFu **********************************/
