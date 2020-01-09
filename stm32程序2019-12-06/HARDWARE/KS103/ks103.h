#ifndef __KS103_H
#define __KS103_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
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
