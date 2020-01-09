#include "led.h" 
#include "motor.h"

u16 crc_calculation(u8 dianji[])
{
	
unsigned char len = 10;
unsigned long temp = 0;
unsigned int crc;
unsigned char i;
u8 DataArray[10];
unsigned char *ptr = DataArray;
	DataArray[0]=dianji[3];
	DataArray[1]=dianji[2];
	DataArray[2]=dianji[5];
	DataArray[3]=dianji[4];
	DataArray[4]=dianji[7];
	DataArray[5]=dianji[6];
	DataArray[6]=dianji[9];
	DataArray[7]=dianji[8];
	DataArray[8]=dianji[11];
	DataArray[9]=dianji[10];

while( len-- ) {
for(i = 0x80; i != 0; i = i >> 1) {
temp = temp * 2;
if((temp & 0x10000) != 0)
temp = temp ^ 0x11021;
if((*ptr & i) != 0)
temp = temp ^ (0x10000 ^ 0x11021);
}
ptr++;
}
crc = temp;
return crc;
}

void motor_reclear(void)
{
	u8 i;
	u8 j;
	u8 dianji1[4]={0x00,0x00,0x00,0x00};
	u8 dianji2[4]={0x00,0x00,0x00,0x00};
	u8 dianji3[4]={0x00,0x00,0x00,0x00};
	u8 dianji4[4]={0x00,0x00,0x00,0x00};
	for(j=0;j<4;j++)
	{
		USART_SendData(USART2, dianji1[j]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<4;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<4;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<4;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}			
}

void motor_init(void)
{
	u8 i;
	u8 j;
	u8 dianji1[15]={0x00,0x90,0x02,0x68,0x04,0x01,0x60,0x60,0x00,0x03,0x00,0x00,0x00,0xBA,0x09};
	u8 dianji2[15]={0x00,0x90,0x02,0x68,0x04,0x02,0x60,0x60,0x00,0x03,0x00,0x00,0x00,0x38,0xD1};
	u8 dianji3[15]={0x00,0x90,0x02,0x68,0x04,0x03,0x60,0x60,0x00,0x03,0x00,0x00,0x00,0x59,0x69};
	u8 dianji4[15]={0x00,0x90,0x02,0x68,0x04,0x04,0x60,0x60,0x00,0x03,0x00,0x00,0x00,0x1D,0x70};
	for(j=1;j<15;j++)
	{
		USART_SendData(USART2, dianji1[j]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=1;i<15;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=1;i<15;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=1;i<15;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}			
}

void enable_init(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0x40,0x60,0x00,0x06,0x00,0x00,0x00,0x22,0x99};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0x40,0x60,0x00,0x06,0x00,0x00,0x00,0xA0,0x41};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0x40,0x60,0x00,0x06,0x00,0x00,0x00,0xC1,0xF9};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0x40,0x60,0x00,0x06,0x00,0x00,0x00,0x85,0xE0};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void start(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0x40,0x60,0x00,0x0F,0x00,0x00,0x00,0xB3,0x07};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0x40,0x60,0x00,0x0F,0x00,0x00,0x00,0x31,0xDF};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0x40,0x60,0x00,0x0F,0x00,0x00,0x00,0x50,0x67};//开始
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0x40,0x60,0x00,0x0F,0x00,0x00,0x00,0x14,0x7E};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void qianjin(u32 speed11,u32 speed22,u32 speed33,u32 speed44)
{
	u8 i;	
	u16 crc=0;
	u32 speed1,speed2,speed3,speed4;
	speed1=speed11;
	speed2=speed22;
	speed3=speed33;
	speed4=speed44;

	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0x51,0xdb};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x2D,0xB6};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x4C,0x0E};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0xF6,0xA2};
	//speed1 motor1
	dianji1[8]=dianji1[9]=dianji1[10]=dianji1[11]=0xFF;
	dianji1[8]&=speed1;
	dianji1[9]&=speed1>>8;
	dianji1[10]&=speed1>>16;
	dianji1[11]&=speed1>>24;
  crc=crc_calculation(dianji1);
	dianji1[12]=dianji1[13]=0xFF;
	dianji1[12]&=crc;
	dianji1[13]&=crc>>8;
	
	//speed2 motor2
	dianji2[8]=dianji2[9]=dianji2[10]=dianji2[11]=0xFF;
	dianji2[8]&=speed2;
	dianji2[9]&=speed2>>8;
	dianji2[10]&=speed2>>16;
	dianji2[11]&=speed2>>24;
  crc=crc_calculation(dianji2);
	dianji2[12]=dianji2[13]=0xFF;
	dianji2[12]&=crc;
	dianji2[13]&=crc>>8;

 //speed2 motor3
	dianji3[8]=dianji3[9]=dianji3[10]=dianji3[11]=0xFF;
	dianji3[8]&=speed3;
	dianji3[9]&=speed3>>8;
	dianji3[10]&=speed3>>16;
	dianji3[11]&=speed3>>24;
  crc=crc_calculation(dianji3);
	dianji3[12]=dianji3[13]=0xFF;
	dianji3[12]&=crc;
	dianji3[13]&=crc>>8;

	//speed1 motor4
	dianji4[8]=dianji4[9]=dianji4[10]=dianji4[11]=0xFF;
	dianji4[8]&=speed4;
	dianji4[9]&=speed4>>8;
	dianji4[10]&=speed4>>16;
	dianji4[11]&=speed4>>24;
  crc=crc_calculation(dianji4);
	dianji4[12]=dianji4[13]=0xFF;
	dianji4[12]&=crc;
	dianji4[13]&=crc>>8;
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}


void chaxun_encoder(void)
{
	u8 i;	
	u8 dianji1[10]={0x90,0x02,0x60,0x02,0x01,0x64,0x60,0x00,0x29,0x5A};
	u8 dianji2[10]={0x90,0x02,0x60,0x02,0x02,0x64,0x60,0x00,0x79,0x03};
	u8 dianji3[10]={0x90,0x02,0x60,0x02,0x03,0x64,0x60,0x00,0x49,0x34};//不需要
	u8 dianji4[10]={0x90,0x02,0x60,0x02,0x04,0x64,0x60,0x00,0xD9,0xB1};//不需要
	for(i=0;i<10;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<10;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	
	for(i=0;i<10;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<10;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
	
}
void chaxun_velocity(void)
{
	u8 i;	
	u8 dianji1[10]={0x90,0x02,0x60,0x02,0x01,0xD3,0x30,0x01,0x11,0x93};
	u8 dianji2[10]={0x90,0x02,0x60,0x02,0x02,0xD3,0x30,0x01,0x41,0xCA};
	u8 dianji3[10]={0x90,0x02,0x60,0x02,0x03,0xD3,0x30,0x01,0x71,0xFD};
	u8 dianji4[10]={0x90,0x02,0x60,0x02,0x04,0xD3,0x30,0x01,0xE1,0x78};
	
	for(i=0;i<10;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<10;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	
	
	for(i=0;i<10;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<10;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
	
}

void stop(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0x40,0x60,0x00,0x0F,0x01,0x00,0x00,0x07,0x71};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0x40,0x60,0x00,0x0F,0x01,0x00,0x00,0x85,0xA9};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0x40,0x60,0x00,0x0F,0x01,0x00,0x00,0xE4,0x11};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0x40,0x60,0x00,0x0F,0x01,0x00,0x00,0xA0,0x08};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void houtui(u32 speedl,u32 speedr)
{
	u8 i;	
	u16 crc=0;
	u32 speed1,speed2;
	speed1=-speedl;
	speed2=speedr;
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0xAF,0x6E};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xD3,0x03};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xB2,0xBB};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x08,0x17};
	//speed1 motor1
	dianji1[8]=dianji1[9]=dianji1[10]=dianji1[11]=0xFF;
	dianji1[8]&=speed1;
	dianji1[9]&=speed1>>8;
	dianji1[10]&=speed1>>16;
	dianji1[11]&=speed1>>24;
  crc=crc_calculation(dianji1);
	dianji1[12]=dianji1[13]=0xFF;
	dianji1[12]&=crc;
	dianji1[13]&=crc>>8;
	//dianji1[12]=~dianji1[12];
	//dianji1[13]=dianji1[13]+1;
	//speed2 motor2
	dianji2[8]=dianji2[9]=dianji2[10]=dianji2[11]=0xFF;
	dianji2[8]&=speed2;
	dianji2[9]&=speed2>>8;
	dianji2[10]&=speed2>>16;
	dianji2[11]&=speed2>>24;
  crc=crc_calculation(dianji2);
	dianji2[12]=dianji2[13]=0xFF;
	dianji2[12]&=crc;
	dianji2[13]&=crc>>8;
 //speed2 motor3
	for(i=8;i<12;i++)
	{
		dianji3[i]=dianji2[i];
	}
  crc=crc_calculation(dianji3);
	dianji3[12]=dianji3[13]=0xFF;
	dianji3[12]&=crc;
	dianji3[13]&=crc>>8;
	//speed2 motor4
	for(i=8;i<12;i++)
	{
		dianji4[i]=dianji1[i];
	}
  crc=crc_calculation(dianji4);
	dianji4[12]=dianji4[13]=0xFF;
	dianji4[12]&=crc;
	dianji4[13]&=crc>>8;
	//dianji4[12]=~dianji4[12];
	//dianji4[13]=dianji4[13]+3;
	
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void zuozhuan(u32 speedl,u32 speedr)
{
	u8 i;	
	u16 crc=0;
	u32 speed1,speed2;
	speed1=-speedl;
	speed2=-speedr;
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0xAF,0x6E};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x2D,0xB6};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x4C,0x0E};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x08,0x17};
	//speed2 motor1
	dianji1[8]=dianji1[9]=dianji1[10]=dianji1[11]=0xFF;
	dianji1[8]&=speed1;
	dianji1[9]&=speed1>>8;
	dianji1[10]&=speed1>>16;
	dianji1[11]&=speed1>>24;
  crc=crc_calculation(dianji1);
	dianji1[12]=dianji1[13]=0xFF;
	dianji1[12]&=crc;
	dianji1[13]&=crc>>8;
//	dianji1[12]=~dianji1[12];
//	dianji1[13]=dianji1[13]+1;
	//speed2 motor2
	dianji2[8]=dianji2[9]=dianji2[10]=dianji2[11]=0xFF;
	dianji2[8]&=speed2;
	dianji2[9]&=speed2>>8;
	dianji2[10]&=speed2>>16;
	dianji2[11]&=speed2>>24;
  crc=crc_calculation(dianji2);
	dianji2[12]=dianji2[13]=0xFF;
	dianji2[12]&=crc;
	dianji2[13]&=crc>>8;
//	dianji2[12]=~dianji2[12];
//	dianji2[13]=dianji2[13]+1;
 //speed2 motor3
	for(i=8;i<12;i++)
	{
		dianji3[i]=dianji2[i];
	}
  crc=crc_calculation(dianji3);
	dianji3[12]=dianji3[13]=0xFF;
	dianji3[12]&=crc;
	dianji3[13]&=crc>>8;
//	dianji3[12]=~dianji3[12];
//	dianji3[13]=dianji3[13]+1;
	//speed2 motor4
	for(i=8;i<12;i++)
	{
		dianji4[i]=dianji1[i];
	}
  crc=crc_calculation(dianji4);
	dianji4[12]=dianji4[13]=0xFF;
	dianji4[12]&=crc;
	dianji4[13]&=crc>>8;
	//dianji4[12]=~dianji4[12];
	//dianji4[13]=dianji4[13]+3;
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void youzhuan(u32 speedl,u32 speedr)
{
	u8 i;
  u16 crc=0;
	u32 speed1,speed2;
	speed1=speedl;
	speed2=speedr;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0x51,0xdb};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xD3,0x03};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xB2,0xBB};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0xF6,0xA2};
	
	//speed1 motor1
	dianji1[8]=dianji1[9]=dianji1[10]=dianji1[11]=0xFF;
	dianji1[8]&=speed1;
	dianji1[9]&=speed1>>8;
	dianji1[10]&=speed1>>16;
	dianji1[11]&=speed1>>24;
  crc=crc_calculation(dianji1);
	dianji1[12]=dianji1[13]=0xFF;
	dianji1[12]&=crc;
	dianji1[13]&=crc>>8;
	//speed1 motor2
	dianji2[8]=dianji2[9]=dianji2[10]=dianji2[11]=0xFF;
	dianji2[8]&=speed2;
	dianji2[9]&=speed2>>8;
	dianji2[10]&=speed2>>16;
	dianji2[11]&=speed2>>24;
  crc=crc_calculation(dianji2);
	dianji2[12]=dianji2[13]=0xFF;
	dianji2[12]&=crc;
	dianji2[13]&=crc>>8;
 //speed1 motor3
	for(i=8;i<12;i++)
	{
		dianji3[i]=dianji2[i];
	}
  crc=crc_calculation(dianji3);
	dianji3[12]=dianji3[13]=0xFF;
	dianji3[12]&=crc;
	dianji3[13]&=crc>>8;
	//speed1 motor4
	for(i=8;i<12;i++)
	{
		dianji4[i]=dianji1[i];
	}
  crc=crc_calculation(dianji4);
	dianji4[12]=dianji4[13]=0xFF;
	dianji4[12]&=crc;
	dianji4[13]&=crc>>8;
	
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void zuoyi(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0xAF,0x6E};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x2D,0xB6};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xB2,0xBB};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0xF6,0xA2};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void youyi(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0xFF,0x60,0x00,0xe8,0x03,0x00,0x00,0x51,0xdb};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0xFF,0x60,0x00,0xE8,0x03,0x00,0x00,0xD3,0x03};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x4C,0x0E};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0xFF,0x60,0x00,0x18,0xFC,0xFF,0xFF,0x08,0x17};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void cuowu(void)
{
	u8 i;	
	u8 dianji1[14]={0x90,0x02,0x68,0x04,0x01,0x40,0x60,0x00,0x80,0x00,0x00,0x00,0xD8,0x10};
	u8 dianji2[14]={0x90,0x02,0x68,0x04,0x02,0x40,0x60,0x00,0x80,0x00,0x00,0x00,0x5A,0xC8};
	u8 dianji3[14]={0x90,0x02,0x68,0x04,0x03,0x40,0x60,0x00,0x80,0x00,0x00,0x00,0x3B,0x70};
	u8 dianji4[14]={0x90,0x02,0x68,0x04,0x04,0x40,0x60,0x00,0x80,0x00,0x00,0x00,0x7F,0x69};
	for(i=0;i<14;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<14;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<14;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}

void chaxun(void)
{
	u8 i;	
	u8 dianji1[10]={0x90,0x02,0x60,0x02,0x01,0xD3,0x30,0x01,0x11,0x93};
	u8 dianji2[10]={0x90,0x02,0x60,0x02,0x02,0xD3,0x30,0x01,0x41,0xCA};
	u8 dianji3[10]={0x90,0x02,0x60,0x02,0x03,0xD3,0x30,0x01,0x71,0xFD};
	u8 dianji4[10]={0x90,0x02,0x60,0x02,0x04,0xD3,0x30,0x01,0xE1,0x78};
	for(i=0;i<10;i++)
	{
		USART_SendData(USART2, dianji1[i]);         //向串口2发送数据
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
	}
	for(i=0;i<10;i++)
	{
		USART_SendData(USART3, dianji2[i]);         //向串口3发送数据
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<10;i++)
	{
		USART_SendData(UART4, dianji3[i]);         //向串口4发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//等待发送结束
	}		
	for(i=0;i<10;i++)
	{
		USART_SendData(USART6, dianji4[i]);         //向串口6发送数据
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//等待发送结束
	}	
}





	

