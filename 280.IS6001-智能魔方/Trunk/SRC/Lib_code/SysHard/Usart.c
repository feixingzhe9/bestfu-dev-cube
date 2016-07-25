/******************************Copyright BestFu 2014-05-14*****************************
	文件： UART.c
	说明： 串口操作相关函数
	编译： Keil uVision4 V4.54.0.0
	版本： v1.0
	编写： Unarty
	日期： 2014.07.03
*****************************************************************************/
#include "sys.h"
#include "usart.h"	 
#include "Bf_type.h"
#include "cmdProcess.h"
#include "GPIO.h"

//********************************************************************************
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#ifdef _DEBUG
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完1毕    
	USART2->DR = (u8) ch;    
	return ch;
}
#endif 
/*******************************************************************
函 数 名：	UART_Init
功能说明：	串口初始化
参	  数：	pclk2： 时钟频率（单位M）
			bound： 通信波特率
返 回 值：	无
*******************************************************************/ 
void UART_Init(u32 pclk2, u32 bound)
{  			  
	RCC->APB1RSTR |= 1<<17;   	//复位串口2
	RCC->APB1RSTR &= ~(1<<17);	//停止复位 

	RCC->APB1ENR |= 1<<17;  	//使能串口时钟 

	GPIOx_Cfg(GPIO_A, pin_2, AF_PP_40M);  //TX
	GPIOx_Cfg(GPIO_A, pin_3, AF_PP_40M); //RX 变态设置 和F103最大差别
	GPIOx_AF (GPIO_A, pin_2|pin_3, AF_USART2);
		   	   
	//波特率设置
 	USART2->BRR = (pclk2*1000000)/(bound); // 波特率设置	 
	USART2->CR1 |= 0X200C;  	//1位停止,无校验位.

	USART2->CR1 |= 1<<8;    	//PE中断使能
	USART2->CR1 |= 1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3, 3, USART2_IRQn, 2);//组2，最低优先级 
}
 	
 	
/*******************************************************************
函 数 名：	UART_Send
功能说明：	串口数据发送
参	  数：	*data: 要发送的数内容
			len :	数据长度
返 回 值：	发送结果 TRUE/FALSE
*******************************************************************/ 
u8 UART_Send(u8 *data, u8 len, u8 level)
{
	while (len--)
	{
		if (!UART_Write(*data++))
		{
			UART_Init(72, 9600);
		}
	}

	return TRUE;
}

/*******************************************************************
函 数 名：	UART_WriteBit
功能说明：	串口写入一个字节数据 
参	  数：	ch:	要写入的数据
返 回 值：	写入结果TRUE/FALSE
*******************************************************************/ 
u8 UART_Write(u8 c)
{   
	u32 t;
	
	for (t = 0; ((USART2->SR&0X40) == 0)&&(t < 20000); t++)   //等待数据发送完毕
	{}	
		
	if (t < 20000)  //未超时
	{
		USART2->DR = (u8)c;
		return TRUE;
	}
	      
	return FALSE;
}

/*******************************************************************
函 数 名：	USART1_IRQHandler
功能说明：	串口中断处理
参	  数：	无
返 回 值：	无
*******************************************************************/ 
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		fifo_putc(&rcv433fifo, USART2->DR);
	}

} 
/****************File End***********Copyright BestFu 2014-05-14*****************/
