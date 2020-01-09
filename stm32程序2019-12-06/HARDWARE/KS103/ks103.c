#include "myiic.h"
#include "delay.h"




//����IIC��ʼ�ź�
void IIC_KS103_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(10);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(10);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_KS103_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(10);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(10);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_KS103_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(6);	   
	IIC_SCL=1;delay_us(6);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_KS103_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_KS103_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_KS103_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_KS103_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(10);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(10); 
		IIC_SCL=0;	
		delay_us(10);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_KS103_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(10);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        IIC_KS103_NAck();//����nACK
    else
        IIC_KS103_Ack(); //����ACK   
    return receive;
}
u8 KS103_ReadOneByte(u8 address, u8 reg)
{
u8 temp=0;
IIC_KS103_Start();
IIC_KS103_Send_Byte(address); //?????
IIC_KS103_Wait_Ack();
IIC_KS103_Send_Byte(reg); //?????
IIC_KS103_Wait_Ack();
IIC_KS103_Start();
IIC_KS103_Send_Byte(address + 1); //??????
IIC_KS103_Wait_Ack();
delay_us(100); //?????????!!!
temp=IIC_KS103_Read_Byte(0); //????3
IIC_KS103_Stop();//????????
return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{
IIC_KS103_Start();
IIC_KS103_Send_Byte(address); //?????
IIC_KS103_Wait_Ack();
IIC_KS103_Send_Byte(reg);//?????
IIC_KS103_Wait_Ack();
IIC_KS103_Send_Byte(command); //?????
IIC_KS103_Wait_Ack();
IIC_KS103_Stop();//????????
}


u16 detect(u8 address,u8 command) //0xe8(address) + 0xb0(command)
{
u16 distance=0;
KS103_WriteOneByte(address,0X02,command);
//delay_ms(80);
	delay_ms(50);
	//while(!IIC_SCL);
distance = KS103_ReadOneByte(address, 0x02);
distance <<= 8;
distance += KS103_ReadOneByte(address, 0x03);
return distance; //return 16 bit distance in millimeter
}

void KS103_change_address(u8 old_address,u8 new_address)
{
//delay_ms(2000); // Protect the eeprom ,you can delete this sentence
KS103_WriteOneByte(old_address,2,0x9a);
delay_ms(1);
KS103_WriteOneByte(old_address,2,0x92);
delay_ms(1);
KS103_WriteOneByte(old_address,2,0x9e);
delay_ms(1);
KS103_WriteOneByte(old_address,2, new_address);
delay_ms(100);
}
