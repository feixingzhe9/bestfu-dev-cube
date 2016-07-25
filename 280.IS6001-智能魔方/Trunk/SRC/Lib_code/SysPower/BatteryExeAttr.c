/***************************Copyright BestFu ***********************************
**  文    件：  BatteryExeAttr.c
**  功    能：  <<属性层>> 电池属性接口 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.29
**  说    明：  
**  V1.1
    >> 取消读取电量值时每次测量  改为上电首次测量的电量值
*******************************************************************************/
#include "BatteryExeAttr.h"
#include "ADC.h"
#include "Battery.h"
#include "Thread.h"
#include "GPIO.h"
#include "cube6.h"

extern void Standby_Mode(void);
static void bubble_sort(u8 *x, u32 n);
/*******************************************************************************
**函    数： TP4056
**功    能： TP4056初始化
**参    数： void  
**返    回： void
*******************************************************************************/
static void TP4056(void)
{
	GPIOx_Cfg(CHRG_PORT, CHRG_PIN, IN_FLOATING); //IN_UP
	GPIOx_Cfg(STDBY_PORT, STDBY_PIN, IN_FLOATING); //IN_UP
}
/*******************************************************************************
**函    数： Battery_Init
**功    能： 电池属性初始化
**参    数： void  
**返    回： void
*******************************************************************************/
void Battery_Init(void)
{
	u8 ucCount = 0;
	u8 ucLoopCount = 0;
    u8 voltageTab[5] = {0};
	
	ADC_Init();
	TP4056();
	while(ucLoopCount++ < 8)
	{
		if(Battery_GetPercent(&gPowerPercent))
		{
			voltageTab[ucCount++] = gPowerPercent;
			if(ucCount > 5)
			{
				bubble_sort(voltageTab,5);
				gPowerPercent = (voltageTab[1]+voltageTab[2]+voltageTab[3])/3;
				if(gPowerPercent < 10)
				{
					Standby_Mode();
				}
				else if(gPowerPercent < 30)
				{
					CubeHandle.LowVoltageTips  = 1; 
				}
				break;
			}
		}
	}
}

/*******************************************************************************
函 数 名:  	static void bubble_sort(u8 *x, u32 n)
功能说明: 	冒泡法排序
参    数:  	x:表示需要排序的数据首指针  n:表示个数
返 回 值:  	无
*******************************************************************************/
static void bubble_sort(u8 *x, u32 n)
{
    int j, k, h;
    u8 t;
    for (h=n-1; h>0; h=k)                                            /**< 循环到没有比较范围 */
    {
         for (j=0, k=0; j<h; j++)                                    /**< 每次预置k=0，循环扫描后更新k */
         {
            if (*(x+j) > *(x+j+1))                               /**< 大的放在后面，小的放到前面 */
            {
                 t = *(x+j);
                 *(x+j) = *(x+j+1);
                 *(x+j+1) = t;                                   /**< 完成交换 */
                 k = j;                                          /**< 保存最后下沉的位置。这样k后面的都是排序排好了的 */
            }
         }
    }
}

/*******************************************************************************
**函    数： Get_PowerPercent_Attr
**功    能： 获取电池电量百分比
**参    数： *pData        -- 输入参数指针
**           *rlen         -- 返回参数长度
**           *rData        -- 返回参数指针
**返    回： COMPLETE：成功    
*******************************************************************************/
MsgResult_t Get_PowerPercent_Attr(UnitPara_t *pData, u8 *rLen, u8 *rData)
{
	if (1 == Battery_GetPercent(&gPowerPercent))
	{
		*rData = gPowerPercent;
		*rLen = 1;   
		return COMPLETE;        
	}
	else
	{
		return CMD_EXE_ERR;
	}
}

/*******************************************************************************
**函    数： Get_ChargeState_Attr
**功    能： 获取充电状态
**参    数： *pData        -- 输入参数指针
**           *rlen         -- 返回参数长度
**           *rData        -- 返回参数指针
**返    回： COMPLETE：成功    
**说    明： (此功能暂未实现)
*******************************************************************************/
MsgResult_t Get_ChargeState_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara)
{
    return COMPLETE;
}

/***************************Copyright BestFu **********************************/
