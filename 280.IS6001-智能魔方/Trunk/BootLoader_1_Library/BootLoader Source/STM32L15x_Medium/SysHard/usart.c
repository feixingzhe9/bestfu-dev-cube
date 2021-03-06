/***************************Copyright xxxxx 2015-12-30**************************
**  文    件：  usart.c
**  功    能：  串口底层驱动函数
**  编    译：  MDK-ARM Professional Versoin: 5.12.0.0
**  版    本：  V1.0
**  编    写：  Jay
**  创建日期：  2015-12-29
**  修改日期：  无
**  说    明：  初版
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "FifoDataProc.h"
#include "FaultManage.h"
#include "PhyMsgProc.h"
#include "stm32l1xx.h"
#include "sys.h"
#include "system_stm32l1xx.h"
#include "usart.h"

#if (USART_INTERFACE_EN > 0u )

//Private function declaration
static BF_INT08U USART_Write(BF_INT08U c);

/*******************************************************************************
函 数 名：	USART_Init
功能说明：	串口初始化
参	  数：	pclk2： 时钟频率（单位M）
			bound： 通信波特率
返 回 值：	无
*******************************************************************************/
void USART_Init(BF_INT32U bound)
{
#if 0
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);//配置串口波特率
    RCC->APB2RSTR |= 1<<14;   	//复位串口1
    RCC->APB2RSTR &= ~(1<<14);	//停止复位

    RCC->AHBENR  |= 1<<0;   	//使能PORTA口时钟
    RCC->APB2ENR |= 1<<14;  	//使能串口时钟

    GPIOA->MODER &= 0xFFC3FFFF; //PA9,PA10管脚复用
    GPIOA->MODER |= 0x00280000;

    GPIOA->OTYPER&= 0xFFFFF9FF; //设置管脚推挽输出

    GPIOA->OSPEEDR&= 0xFFC3FFFF; //设置管脚速度40M
    GPIOA->OSPEEDR|= 0x003C0000;

    GPIOA->PUPDR  &= 0xFFC3FFFF; //管脚不上下拉

    GPIOA->AFR[1] &= 0xFFFFFFFF; //设置复用功能
    GPIOA->AFR[1] |= 0x00000770;

    //波特率设置
    USART1->BRR = (RCC_ClocksStatus.PCLK2_Frequency)/(bound); // 波特率设置
    USART1->CR1 |= 0X200C;  	//1位停止,无校验位.

    USART1->CR1 |= 1<<8;    	//PE中断使能
    USART1->CR1 |= 1<<5;    	//接收缓冲区非空中断使能
    MY_NVIC_Init(3, 3, USART1_IRQn, 2);//组2，最低优先级
#endif

    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);//配置串口波特率
    RCC->APB1RSTR |= 1<<17;   	//复位串口2
    RCC->APB1RSTR &= ~(1<<17);	//停止复位

    RCC->AHBENR  |= 1<<0;   	//使能PORTA口时钟
    RCC->APB1ENR |= 1<<17;  	//使能串口时钟

    GPIOA->MODER &= 0xFFFFF0F; //PA2,PA3管脚复用
    GPIOA->MODER |= 0x00000A0;

    GPIOA->OTYPER&= 0xFFFFFFF3; //设置管脚推挽输出

    GPIOA->OSPEEDR&= 0xFFFFFF0F; //设置管脚速度40M
    GPIOA->OSPEEDR|= 0x000000F0;

    GPIOA->PUPDR  &= 0xFFFFFF0F; //管脚不上下拉

    GPIOA->AFR[0] &= 0xFFFFFFFF; //设置复用功能
    GPIOA->AFR[0] |= 0x00007700;

    //波特率设置
    USART2->BRR = (RCC_ClocksStatus.PCLK1_Frequency)/(bound); // 波特率设置
    USART2->CR1 |= 0X200C;  	//1位停止,无校验位.

    USART2->CR1 |= 1<<8;    	//PE中断使能
    USART2->CR1 |= 1<<5;    	//接收缓冲区非空中断使能
	USART2->SR   = ~0x40;
    MY_NVIC_Init(3, 3, USART2_IRQn, 2);//组2，最低优先级

}

/*******************************************************************************
函 数 名：	USART_Send
功能说明：	串口数据发送
参	  数：	*data: 要发送的数内容
			len :	数据长度
返 回 值：	发送结果 TRUE/FALSE
*******************************************************************************/
BF_INT08U USART_Send(BF_INT08U *data, BF_INT08U len, BF_INT08U level)
{
    while (len--)
    {
        if (!USART_Write(*data++))
        {
            return FALSE;;
        }
    }
    return TRUE;
}

/*******************************************************************************
函 数 名：	UART_WriteBit
功能说明：	串口写入一个字节数据
参	  数：	ch:	要写入的数据
返 回 值：	写入结果TRUE/FALSE
*******************************************************************************/
static BF_INT08U USART_Write(BF_INT08U c)
{
    BF_INT32U  t;

    for (t = 0; ((USART2->SR&0X40) == 0)&&(t < 20000); t++)   //等待数据发送完毕
    {}

    if (t < 20000)  //未超时
    {
        USART2->DR = (BF_INT08U)c;
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
函 数 名：	USART1_IRQHandler
功能说明：	串口中断处理
参	  数：	无
返 回 值：	无
*******************************************************************************/
void USART2_IRQHandler(void)
{
    if(USART2->SR&(1<<5))//接收到数据
    {
        CPU_SR_ALLOC();
        ENTER_CRITICAL();
        fifo_putc(&phy_rcvUsartfifo, USART2->DR);
        EXIT_CRITICAL();
    }
    USART2->SR &=~(1<<5);
}
#endif
/***********************************END OF FILE***********************************/
