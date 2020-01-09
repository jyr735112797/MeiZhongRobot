#ifndef __KS103_H
#define __KS103_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 
u8 KS103_ReadOneByte(u8 address, u8 reg);
u16 detect(u8 address,u8 command) ;
void KS103_WriteOneByte(u8 address,u8 reg,u8 command)		;	
void IIC_KS103_Start(void);

void IIC_KS103_Stop(void);

u8 IIC_KS103_Wait_Ack(void);

void IIC_KS103_Ack(void);
    
void IIC_KS103_NAck(void);
		  
void IIC_KS103_Send_Byte(u8 txd);
void KS103_change_address(u8 old_address,u8 new_address);
u8 IIC_KS103_Read_Byte(unsigned char ack);
#endif
