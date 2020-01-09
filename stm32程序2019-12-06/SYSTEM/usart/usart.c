#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


#if 1
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
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[USART2_REC_LEN];  
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 UART5_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART6_RX_BUF[USART6_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
u16 USART2_RX_STA=0;       //接收状态标记	
u16 USART3_RX_STA=0;       //接收状态标记	
u16 UART4_RX_STA=0;       //接收状态标记
u16 UART5_RX_STA=0;       //接收状态标记
u16 USART6_RX_STA=0;       //接收状态标记	

u8 test1=0;
u8 test2=0;
u8 test3=0;
u8 test4=0;
u8 test5=0;
u8 test6=0;
u8 flag_1=0;
u8 flag_2=0;
u8 flag_3=0;
u8 flag_4=0;
u8 flag_5=0;
u8 flag_6=0;
#if EN_USART1_RX   //如果使能了接收

void uart1_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO1_InitStructure;
	USART_InitTypeDef USART1_InitStructure;
	NVIC_InitTypeDef NVIC1_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1	
	//USART1端口配置
  GPIO1_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO1_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO1_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO1_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO1_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART1_InitStructure.USART_BaudRate = bound;//波特率设置
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART1_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART1_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
//	USART_ClearFlag(USART1, USART_FLAG_TC);	
	
	//Usart1 NVIC 配置
  NVIC1_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC1_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC1_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC1_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC1_InitStructure);	//根据指定的参数初始化VIC寄存器、	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
		   	else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
} 

#endif	

//串口2
#if EN_USART2_RX   //如果使能了接收

void uart2_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO2_InitStructure;
	USART_InitTypeDef USART2_InitStructure;
	NVIC_InitTypeDef NVIC2_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1); //GPIOA10复用为USART1	
	//USART1端口配置
  GPIO2_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO2_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO2_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO2_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO2_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO2_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART2_InitStructure.USART_BaudRate = bound;//波特率设置
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART2_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART2_InitStructure); //初始化串口1
	
  USART_Cmd(USART2, ENABLE);  //使能串口2 
	
//	USART_ClearFlag(USART2, USART_FLAG_TC);

	//Usart1 NVIC 配置
  NVIC2_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC2_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC2_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC2_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC2_InitStructure);	//根据指定的参数初始化VIC寄存器、	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		
	
		//USART_SendData(USART1,Res);      
  //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		
		
if((USART2_RX_STA&0x8000)==0x8000)//接收开始
		{
			if(Res==0x90)
			flag_2+=1;
			if((test2<26)&&(flag_2!=2))
			{
				USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
			  USART2_RX_STA++;
			  if((USART2_RX_STA&0x3fff)>(USART2_REC_LEN-1))
				USART2_RX_STA=0;
			
			test2++;
			}
			else 
			{
				USART2_RX_STA&=0x8fff;	
			flag_2=0;
			}
		}
		else
		{			
			if((USART2_RX_STA&0x4000)==0x4000)//接收到了0x90
			{
				if(Res!=0x02)USART2_RX_STA=0;//接收错误,重新开始
				else 
				{
				 USART2_RX_STA|=0x8000;	//接收完成了 
				 test2=0;
				}
			}
			else //还没收到0X90
			{	
				if(Res==0x90)USART2_RX_STA|=0x4000;
				else USART2_RX_STA=0; 
			}
		} 		
  } 
} 

#endif	


#if EN_USART3_RX   //如果使能了接收

void uart3_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO3_InitStructure;
	USART_InitTypeDef USART3_InitStructure;
	NVIC_InitTypeDef NVIC3_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟	
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1
  //USART3端口配置
  GPIO3_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO3_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO3_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO3_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO3_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO3_InitStructure); //初始化PA9，PA10
  //USART3 初始化设置
	USART3_InitStructure.USART_BaudRate = bound;//波特率设置
	USART3_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART3_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART3_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART3_InitStructure); //初始化串口3

	USART_Cmd(USART3, ENABLE);  //使能串口3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC 配置
  NVIC3_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC3_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC3_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC3_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC3_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);//开启相关中断
}
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	  u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		//USART_SendData(USART3,Res);      
 // while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		
		if((USART3_RX_STA&0x8000)==0x8000)//接收开始
		{
			if(Res==0x90)
			flag_3+=1;
			if((test3<26)&&(flag_3!=2))
			{USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
			USART3_RX_STA++;
			if((USART3_RX_STA&0x3fff)>(USART3_REC_LEN-1))USART3_RX_STA=0;
			
			test3++;}
			else 
			{
			USART3_RX_STA&=0x8fff;	
			flag_3=0;
			}
		}
		else
		{			
			if((USART3_RX_STA&0x4000)==0x4000)//接收到了0x90
			{
				if(Res!=0x02)USART3_RX_STA=0;//接收错误,重新开始
				else 
				{
				 USART3_RX_STA|=0x8000;	//接收完成了 
				 test3=0;
				}
			}
			else //还没收到0X90
			{	
				if(Res==0x90)USART3_RX_STA|=0x4000;
				else USART3_RX_STA=0; 
			}
		} 		
  } 
}

#endif

#if EN_UART4_RX   //如果使能了接收
void uart4_init(u32 bound){
	GPIO_InitTypeDef GPIO4_InitStructure;
	USART_InitTypeDef USART4_InitStructure;
	NVIC_InitTypeDef NVIC4_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10复用为USART1
  //USART3端口配置
  GPIO4_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO4_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO4_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO4_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO4_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO4_InitStructure); //初始化PA9，PA10
  //USART3 初始化设置
	USART4_InitStructure.USART_BaudRate = bound;//波特率设置
	USART4_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART4_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART4_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART4_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART4_InitStructure); //初始化串口3

	USART_Cmd(UART4, ENABLE);  //使能串口3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC 配置
  NVIC4_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC4_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC4_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC4_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC4_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(UART4,USART_IT_RXNE, ENABLE);//开启相关中断
}

void UART4_IRQHandler(void)                	//串口3中断服务程序
{
	 u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
		if((UART4_RX_STA&0x8000)==0x8000)//接收开始
		{
			if(Res==0x90)
			flag_4+=1;
			if((test4<26)&&(flag_4!=2))
			{UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
			UART4_RX_STA++;
			if((UART4_RX_STA&0x3fff)>(UART4_REC_LEN-1))UART4_RX_STA=0;
			
			test4++;}
			else 
			{
				UART4_RX_STA&=0x8fff;	
			flag_4=0;
			}
		}
		else
		{			
			if((UART4_RX_STA&0x4000)==0x4000)//接收到了0x90
			{
				if(Res!=0x02)UART4_RX_STA=0;//接收错误,重新开始
				else 
				{
				 UART4_RX_STA|=0x8000;	//接收完成了 
				 test4=0;
				}
			}
			else //还没收到0X90
			{	
				if(Res==0x90)UART4_RX_STA|=0x4000;
				else UART4_RX_STA=0; 
			}
		} 		
  } 
	
//	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//?????USART??????
//	{	
//	u8	k=USART_ReceiveData(UART4);
//	USART_SendData(USART1,k);      
//  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//	}
//	UART4_RX_STA=0;
}
	
#endif

#if EN_USART6_RX   //如果使能了接收

void uart6_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO6_InitStructure;
	USART_InitTypeDef USART6_InitStructure;
	NVIC_InitTypeDef NVIC6_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10复用为USART1	
	//USART1端口配置
  GPIO6_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9与GPIOA10
	GPIO6_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO6_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO6_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO6_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO6_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART6_InitStructure.USART_BaudRate = bound;//波特率设置
	USART6_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART6_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART6_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART6_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART6_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART6, &USART6_InitStructure); //初始化串口1
	
  USART_Cmd(USART6, ENABLE);  //使能串口1 
	
//	USART_ClearFlag(USART1, USART_FLAG_TC);	
	
	//Usart1 NVIC 配置
  NVIC6_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC6_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC6_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC6_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC6_InitStructure);	//根据指定的参数初始化VIC寄存器、	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
}

void USART6_IRQHandler(void)                	//串口1中断服务程序
{
	  u8 Res;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART6);//(USART1->DR);	//读取接收到的数据
		if((USART6_RX_STA&0x8000)==0x8000)//接收开始
		{
			if(Res==0x90)
			flag_6+=1;
			if((test6<26)&&(flag_6!=2))
			{USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
			USART6_RX_STA++;
			if((USART6_RX_STA&0x3fff)>(USART6_REC_LEN-1))USART6_RX_STA=0;
			
			test6++;}
			else 
			{
				USART6_RX_STA&=0x8fff;	
			flag_6=0;
			}
		}
		else
		{			
			if((USART6_RX_STA&0x4000)==0x4000)//接收到了0x90
			{
				if(Res!=0x02)USART6_RX_STA=0;//接收错误,重新开始
				else 
				{
				 USART6_RX_STA|=0x8000;	//接收完成了 
				 test6=0;
				}
			}
			else //还没收到0X90
			{	
				if(Res==0x90)USART6_RX_STA|=0x4000;
				else USART6_RX_STA=0; 
			}
		} 		
  } 
	/*
	if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET)//?????USART??????
	{	
	u8	k=USART_ReceiveData(USART6);
	USART_SendData(USART1,k);      
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
	USART6_RX_STA=0;
	*/
	
}

#endif

#if EN_UART5_RX   //如果使能了接收
void uart5_init(u32 bound){
	GPIO_InitTypeDef GPIO5_InitStructure;
	USART_InitTypeDef USART5_InitStructure;
	NVIC_InitTypeDef NVIC5_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOA10复用为USART1
  //USART3端口配置
  GPIO5_InitStructure.GPIO_Pin = GPIO_Pin_2 ; //GPIOA9与GPIOA10
	GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO5_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO5_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO5_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO5_InitStructure); //初始化PA9，PA10
	
	GPIO5_InitStructure.GPIO_Pin = GPIO_Pin_12 ; //GPIOA9与GPIOA10
	GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO5_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO5_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO5_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO5_InitStructure); //初始化PA9，PA10
	
  //USART3 初始化设置
	USART5_InitStructure.USART_BaudRate = bound;//波特率设置
	USART5_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART5_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART5_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART5_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART5_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART5, &USART5_InitStructure); //初始化串口3

	USART_Cmd(UART5, ENABLE);  //使能串口3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC 配置
  NVIC5_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口1中断通道
	NVIC5_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC5_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC5_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC5_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_ITConfig(UART5,USART_IT_RXNE, ENABLE);//开启相关中断
}

void UART5_IRQHandler(void)                	//串口3中断服务程序
{
	 u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据
		if((UART5_RX_STA&0x8000)==0x8000)//接收开始
		{
		//	if(Res==0x55)
		//	flag_5+=1;
			if(test5<43)
			{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=Res ;
					UART5_RX_STA++;
					if((UART5_RX_STA&0x3fff)>(UART5_REC_LEN-1))UART5_RX_STA=0;
			
					test5++;
			}
			/*else 
			{
						UART5_RX_STA&=0x8fff;	
						//flag_5=0;
			}*/
		}
		else
		{			
			if((UART5_RX_STA&0x4000)==0x4000)//接收到了0x90
			{
				if(Res!=0x51)UART5_RX_STA=0;//接收错误,重新开始
				else 
				{
				 UART5_RX_STA|=0x8000;	//接收完成了 
				 test5=0;
				}
			}
			else //还没收到0X55
			{	
				if(Res==0x55)UART5_RX_STA|=0x4000;
				else UART5_RX_STA=0; 
			}
		} 	

    // USART1->DR=0x50;         //将接受到的数据发回上位机
		//		while((USART1->SR&0X40)==0);			  //等待发送结束   
		
  } 
	
//	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)//?????USART??????
//	{	
//	u8	k=USART_ReceiveData(UART4);
//	USART_SendData(USART1,k);      
//  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//	}
//	UART4_RX_STA=0;
}
	
#endif
