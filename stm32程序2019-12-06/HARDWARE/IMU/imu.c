#include "imu.h"
#include "led.h"
#include "usart.h"
#include "delay.h"


 /*****************************IMU数据接受************************************************/      
u8 IMU_Get_Data(short *axx,short *ayy,short *azz,short *gxx,short *gyy,short *gzz,short *xpitch,short *yroll,short *zyaw,short *hxx,short *hyy,short *hzz)   
{
	
	u8 len,t;	
	u8 valid;	
	u8 res;  
	if((UART5_RX_STA&0x8000)==0x8000)                           //判断是否接受到完整一帧数据
      {   
        len=0; 
				len=UART5_RX_STA&0x3fff;				
	      /***************** *   	  控制部分  start   	 * ************************/      
         valid = ((UART5_RX_BUF[9] & 0xff ) == 0x55 ) && (( UART5_RX_BUF[10] & 0xff ) == 0x52 );
         if( valid ) //判断数据帧   有效
         { 
						*axx=((u16)UART5_RX_BUF[1]<<8)|UART5_RX_BUF[0];  
						*ayy=((u16)UART5_RX_BUF[3]<<8)|UART5_RX_BUF[2];  
						*azz=((u16)UART5_RX_BUF[5]<<8)|UART5_RX_BUF[4];
					 
					  *gxx=((u16)UART5_RX_BUF[12]<<8)|UART5_RX_BUF[11];  
						*gyy=((u16)UART5_RX_BUF[14]<<8)|UART5_RX_BUF[13];  
						*gzz=((u16)UART5_RX_BUF[16]<<8)|UART5_RX_BUF[15];
					 
					  *xpitch=((u16)UART5_RX_BUF[23]<<8)|UART5_RX_BUF[22];  
						*yroll=((u16)UART5_RX_BUF[25]<<8)|UART5_RX_BUF[24];  
						*zyaw=((u16)UART5_RX_BUF[27]<<8)|UART5_RX_BUF[26];
					 
					  *hxx=((u16)UART5_RX_BUF[34]<<8)|UART5_RX_BUF[33];  
						*hyy=((u16)UART5_RX_BUF[36]<<8)|UART5_RX_BUF[35];  
						*hzz=((u16)UART5_RX_BUF[38]<<8)|UART5_RX_BUF[37];			
					 USART1->DR=UART5_RX_BUF[10];         //将接受到的数据发回上位机
				while((USART1->SR&0X40)==0);			  //等待发送结束 
			UART5_RX_STA=0;          
       } //接收数据的      			 
 return res;			
}
}

void z_zero( )
{
	u8 i;
	
	u8 zero[5]={0xFF,0xAA,0x01,0x04,0x00};
	
	for(i=0;i<5;i++)
	{
		USART_SendData(UART5, zero[i]);         //向串口5发送数据
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//等待发送结束
	}
	
}

