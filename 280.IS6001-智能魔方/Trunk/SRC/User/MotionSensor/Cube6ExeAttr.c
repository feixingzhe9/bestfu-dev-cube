/***************************Copyright BestFu ***********************************
**  文    件：  Cube6ExeAttr.c
**  功    能：  <<属性层>> 魔方六面体属性命令接口
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.05.26
**  修改日期：  2014.08.20
**  说    明：  
**  V1.1
    >> 
*******************************************************************************/
#include "Unitcfg.h"
#include "Cube6.h"  
#include "Cube6ExeAttr.h"   
#include "ShakeMotor.h" 
#include "Thread.h" 
#include "LowPower.h"   
#include "Battery.h"
#include "ADC.h"
#include "BatteryExeAttr.h"

//对属性读写的接口列表
const AttrExe_st Cube6AttrTab[] = 
{
    {0x01, LEVEL_1, NULL      , Get_Cube6Face_Attr    ,0},    
    {0xC8, LEVEL_1, NULL      , Get_PowerPercent_Attr ,0},    //固定属性  获取电池电量
    {0xC9, LEVEL_1, NULL      , Get_ChargeState_Attr  ,0}     //固定属性  获取电池充电状态(充电锂电池才有)    
};

/*******************************************************************************
**函    数： Cube6Init
**功    能： 模块初始化
**参    数： unitID      --单元号
**返    回： void
*******************************************************************************/
void Cube6Init(u8 unitID)
{
    //WKUP_PortInit();
    Battery_Init();
    ShakeMotor_Init();
    MPU6050_Init();
    Cube6_ReadEeprom();
	CubeHandle.PowerOnFlag = 0x01;
    
    Thread_Login(FOREVER, 0, 50, &Cube6_SearchFace);      //1.782ms--3.1ms
    Thread_Login(FOREVER, 0,100, &LowPower_CheckTime);     //睡眠 100*100= 10s
}

/*******************************************************************************
函 数 名:  Cube_UnitAttrNum
功能说明:  计算按键单元属性数量
参    数:  无
返 回 值:  无
*******************************************************************************/
u8 Cube_UnitAttrNum(void)
{
    return ((sizeof(Cube6AttrTab) / sizeof(Cube6AttrTab[0])));
}

/*******************************************************************************
**函    数： Cube6EepromInit
**功    能： 模块所属 eeprom 初始化
**参    数： unitID      --单元号
**返    回： void
*******************************************************************************/
void Cube6EepromInit(u8 unitID)
{
    CubeHandle.LstEnFace = FACE_NULL;
	EEPROM_Write(CUBE_EEPROM_START_ADDR, 1, (u8*)(&CubeHandle.LstEnFace));
}

/*******************************************************************************
**函    数： Get_Cube6Face_Attr
**功    能： 获取面值
**参    数： *pData      --输入参数
             *rLen       --返回参数长度
             *rData      --返回参数存放地址
**返    回： TRUE/FALSE
*******************************************************************************/
MsgResult_t Get_Cube6Face_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
{
    *rLen  = 1;
    *rData = CubeHandle.CurEnFace;
    return COMPLETE;
}

void Cube6Checking_Attr(void)
{
	VibratingAlert(500,200,1);
}

/***************************Copyright BestFu **********************************/
