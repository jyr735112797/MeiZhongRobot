#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[USART2_REC_LEN];  
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 UART5_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u16 USART2_RX_STA=0;       //����״̬���	
u16 USART3_RX_STA=0;       //����״̬���	
u16 UART4_RX_STA=0;       //����״̬���
u16 UART5_RX_STA=0;       //����״̬���
u16 USART6_RX_STA=0;       //����״̬���	

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
#if EN_USART1_RX   //���ʹ���˽���

void uart1_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO1_InitStructure;
	USART_InitTypeDef USART1_InitStructure;
	NVIC_InitTypeDef NVIC1_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1	
	//USART1�˿�����
  GPIO1_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO1_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO1_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO1_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO1_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO1_InitStructure); //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART1_InitStructure.USART_BaudRate = bound;//����������
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART1_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART1_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
//	USART_ClearFlag(USART1, USART_FLAG_TC);	
	
	//Usart1 NVIC ����
  NVIC1_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC1_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC1_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC1_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC1_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
		   	else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 
} 

#endif	

//����2
#if EN_USART2_RX   //���ʹ���˽���

void uart2_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO2_InitStructure;
	USART_InitTypeDef USART2_InitStructure;
	NVIC_InitTypeDef NVIC2_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1); //GPIOA10����ΪUSART1	
	//USART1�˿�����
  GPIO2_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9��GPIOA10
	GPIO2_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO2_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO2_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO2_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO2_InitStructure); //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART2_InitStructure.USART_BaudRate = bound;//����������
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART2_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART2_InitStructure); //��ʼ������1
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 
	
//	USART_ClearFlag(USART2, USART_FLAG_TC);

	//Usart1 NVIC ����
  NVIC2_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC2_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC2_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC2_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC2_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		
	
		//USART_SendData(USART1,Res);      
  //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		
		
if((USART2_RX_STA&0x8000)==0x8000)//���տ�ʼ
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
			if((USART2_RX_STA&0x4000)==0x4000)//���յ���0x90
			{
				if(Res!=0x02)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
				 USART2_RX_STA|=0x8000;	//��������� 
				 test2=0;
				}
			}
			else //��û�յ�0X90
			{	
				if(Res==0x90)USART2_RX_STA|=0x4000;
				else USART2_RX_STA=0; 
			}
		} 		
  } 
} 

#endif	


#if EN_USART3_RX   //���ʹ���˽���

void uart3_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO3_InitStructure;
	USART_InitTypeDef USART3_InitStructure;
	NVIC_InitTypeDef NVIC3_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��	
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10����ΪUSART1
  //USART3�˿�����
  GPIO3_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO3_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO3_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO3_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO3_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO3_InitStructure); //��ʼ��PA9��PA10
  //USART3 ��ʼ������
	USART3_InitStructure.USART_BaudRate = bound;//����������
	USART3_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART3_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART3_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART3_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART3_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART3_InitStructure); //��ʼ������3

	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC ����
  NVIC3_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC3_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC3_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC3_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC3_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);//��������ж�
}
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	  u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		//USART_SendData(USART3,Res);      
 // while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		
		if((USART3_RX_STA&0x8000)==0x8000)//���տ�ʼ
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
			if((USART3_RX_STA&0x4000)==0x4000)//���յ���0x90
			{
				if(Res!=0x02)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
				 USART3_RX_STA|=0x8000;	//��������� 
				 test3=0;
				}
			}
			else //��û�յ�0X90
			{	
				if(Res==0x90)USART3_RX_STA|=0x4000;
				else USART3_RX_STA=0; 
			}
		} 		
  } 
}

#endif

#if EN_UART4_RX   //���ʹ���˽���
void uart4_init(u32 bound){
	GPIO_InitTypeDef GPIO4_InitStructure;
	USART_InitTypeDef USART4_InitStructure;
	NVIC_InitTypeDef NVIC4_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); 

	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10����ΪUSART1
  //USART3�˿�����
  GPIO4_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO4_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO4_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO4_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO4_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO4_InitStructure); //��ʼ��PA9��PA10
  //USART3 ��ʼ������
	USART4_InitStructure.USART_BaudRate = bound;//����������
	USART4_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART4_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART4_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART4_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART4_InitStructure); //��ʼ������3

	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC ����
  NVIC4_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC4_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC4_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC4_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC4_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ITConfig(UART4,USART_IT_RXNE, ENABLE);//��������ж�
}

void UART4_IRQHandler(void)                	//����3�жϷ������
{
	 u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
		if((UART4_RX_STA&0x8000)==0x8000)//���տ�ʼ
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
			if((UART4_RX_STA&0x4000)==0x4000)//���յ���0x90
			{
				if(Res!=0x02)UART4_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
				 UART4_RX_STA|=0x8000;	//��������� 
				 test4=0;
				}
			}
			else //��û�յ�0X90
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

#if EN_USART6_RX   //���ʹ���˽���

void uart6_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO6_InitStructure;
	USART_InitTypeDef USART6_InitStructure;
	NVIC_InitTypeDef NVIC6_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10����ΪUSART1	
	//USART1�˿�����
  GPIO6_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9��GPIOA10
	GPIO6_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO6_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO6_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO6_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO6_InitStructure); //��ʼ��PA9��PA10
   //USART1 ��ʼ������
	USART6_InitStructure.USART_BaudRate = bound;//����������
	USART6_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART6_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART6_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART6_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART6_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART6, &USART6_InitStructure); //��ʼ������1
	
  USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	
//	USART_ClearFlag(USART1, USART_FLAG_TC);	
	
	//Usart1 NVIC ����
  NVIC6_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC6_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC6_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC6_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC6_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
}

void USART6_IRQHandler(void)                	//����1�жϷ������
{
	  u8 Res;
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART6);//(USART1->DR);	//��ȡ���յ�������
		if((USART6_RX_STA&0x8000)==0x8000)//���տ�ʼ
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
			if((USART6_RX_STA&0x4000)==0x4000)//���յ���0x90
			{
				if(Res!=0x02)USART6_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
				 USART6_RX_STA|=0x8000;	//��������� 
				 test6=0;
				}
			}
			else //��û�յ�0X90
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

#if EN_UART5_RX   //���ʹ���˽���
void uart5_init(u32 bound){
	GPIO_InitTypeDef GPIO5_InitStructure;
	USART_InitTypeDef USART5_InitStructure;
	NVIC_InitTypeDef NVIC5_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE); 

	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOA10����ΪUSART1
  //USART3�˿�����
  GPIO5_InitStructure.GPIO_Pin = GPIO_Pin_2 ; //GPIOA9��GPIOA10
	GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO5_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO5_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO5_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO5_InitStructure); //��ʼ��PA9��PA10
	
	GPIO5_InitStructure.GPIO_Pin = GPIO_Pin_12 ; //GPIOA9��GPIOA10
	GPIO5_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO5_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO5_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO5_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO5_InitStructure); //��ʼ��PA9��PA10
	
  //USART3 ��ʼ������
	USART5_InitStructure.USART_BaudRate = bound;//����������
	USART5_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART5_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART5_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART5_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART5_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART5, &USART5_InitStructure); //��ʼ������3

	USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���3
	
// USART_ClearFlag(USART3, USART_FLAG_TC);

//Usart3 NVIC ����
  NVIC5_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����1�ж�ͨ��
	NVIC5_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC5_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC5_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC5_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ITConfig(UART5,USART_IT_RXNE, ENABLE);//��������ж�
}

void UART5_IRQHandler(void)                	//����3�жϷ������
{
	 u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������
		if((UART5_RX_STA&0x8000)==0x8000)//���տ�ʼ
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
			if((UART5_RX_STA&0x4000)==0x4000)//���յ���0x90
			{
				if(Res!=0x51)UART5_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{
				 UART5_RX_STA|=0x8000;	//��������� 
				 test5=0;
				}
			}
			else //��û�յ�0X55
			{	
				if(Res==0x55)UART5_RX_STA|=0x4000;
				else UART5_RX_STA=0; 
			}
		} 	

    // USART1->DR=0x50;         //�����ܵ������ݷ�����λ��
		//		while((USART1->SR&0X40)==0);			  //�ȴ����ͽ���   
		
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
