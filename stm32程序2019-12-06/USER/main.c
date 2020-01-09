#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "motor.h"
#include "timer.h"
#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h" 
#include "ks103.h"
#include "imu.h"
	char flag_500ms=0;
	char flag_50ms=0;

int main(void)
{ 

//u8 len;                           	  //�������ݳ���
  u8 valid=0;                       	  //�ж�֡�Ƿ���Ч
  u32 speed1=0;u32 speed2=0,speed3=0,speed4=0;//������Ϊ1��˳ʱ��1��2��3��4
	u32 max_speed=2000;//���ת��
	u8 command_speed_scale1=0;
	u8 command_speed_scale2=0,command_speed_scale3=0,command_speed_scale4=0;//�ٶ�scale
	u8 key;
	u8 t;
	u8 len;	
	u16 times=0; 
	u16 range;
	u8 encoder_1[4]={0,0,0,0};//����������
	u8 encoder_2[4]={0,0,0,0};
	u8 encoder_3[4]={0,0,0,0};
	u8 encoder_4[4]={0,0,0,0};
	u8 omga_1[4]={0,0,0,0};//ת�ٶ���
	u8 omga_2[4]={0,0,0,0};
	u8 omga_3[4]={0,0,0,0};
	u8 omga_4[4]={0,0,0,0};
	int encorder_one=0,encorder_two=0,encorder_three=0,encorder_four=0;//��������ֵ
	int encorder_left_old=0,encorder_right_old=0;
	int encorder_delta_left=0,encorder_delta_right=0;    //�������������
	int encorder_delta_car=0;
	int omga_one=0,omga_two=0,omga_three=0,omga_four=0;//���ת���ٶ�
	float theta=0.0;         //��λ��
	short aacx,aacy,aacz;		  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short hx,hy,hz;	          //������ԭʼ����
	short pitch,roll,yaw; 		//ŷ����
	float x_angular_velocity,y_angular_velocity,z_angular_velocity,x_linear_acceleration,y_linear_acceleration,z_linear_acceleration,x_magnetic,y_magnetic,z_magnetic,x_pitch,y_roll,z_yaw;//IMU����
	int theta_car,xv,yv,zv,xa,ya,za,xm,ym,zm;
	u8 omga_l[4]={0,0,0,0};
	u8 omga_r[4]={0,0,0,0};
	int omga_left=0,omga_right=0;

	u8 statuspublish[159]={0xCD,0xEB,0xD7,0x9B,//0-3
	                       0x01,0x00,0x00,0x00,0x20,//״̬��־********************************int     4-8
                     		     0x01,0x00,0x00,0x00,0x20,//��ѹ*******************************float    9-13
	                           0xDB,0xD4,0x45,0x41,0x20,//��λ�� 0-360***********************float    14-18
			                       0x7C,0xC5,0x00,0x00,0x20,//����һȦ������������� 50556  *****int      19-23
			                       0x01,0x00,0x00,0x00,0x20,//                         *************int      24-28
			                       0x01,0x00,0x00,0x00,0x20,//��һ���������������/��*************int      29-33
			                       0x01,0x00,0x00,0x00,0x20,//�ڶ����������������/���*****************int     34-38
			                       0x01,0x00,0x00,0x00,0x20,//�������������������/��********************int     39-43
			                       0x01,0x00,0x00,0x00,0x20,//���ĸ��������������/��*********************int     44-48
			                       0xA9,0x71,0xF4,0x41,0x20,//��һ��ת��***************int              49-53
			                       0xA9,0x71,0xF4,0x41,0x20,//�ڶ���                                       54-58
			                       0xA9,0x71,0xF4,0x41,0x20,//������                                      59-63
			                       0xA9,0x71,0xF4,0x41,0x20,//���ĸ�                                      64-68
			                       0x01,0x00,0x00,0x00,0x20,//��������  x����ٶ�*****************float     69-73
			                       0x01,0x00,0x00,0x00,0x20,//y����ٶ�                                   74-78
			                       0x01,0x00,0x00,0x00,0x20,//z����ٶ�                                    79-83
			                       0x01,0x00,0x00,0x00,0x20,//x���ٶ�                                     84-88
			                       0x01,0x00,0x00,0x00,0x20,//y���ٶ�                                     89-93
	                           0x01,0x00,0x00,0x00,0x20,//z���ٶ�                                     94-98
			                       0x01,0x00,0x00,0x00,0x20,//x�Ƕ�                                           99-103
			                       0x01,0x00,0x00,0x00,0x20,// y�Ƕ�                                       104-108
														 0x01,0x00,0x00,0x00,0x20,// z�Ƕ�                                          109-113
														 0xA9,0x71,0xF4,0x41,0x20,//��һ������������   cm***************float    114-118
			                       0xA9,0x71,0xF4,0x41,0x20,//�ڶ���                                       119-123
			                       0xA9,0x71,0xF4,0x41,0x20,//������                                      124-128
			                       0xA9,0x71,0xF4,0x41,0x20,//���ĸ�                                      129-133
														 0xA9,0x71,0xF4,0x41,0x20,//��һ������������   cm***************float    134-138
			                       0xA9,0x71,0xF4,0x41,0x20,//�ڶ���                                       139-143
			                       0xA9,0x71,0xF4,0x41,0x20,//������                                      144-148
			                       0xA9,0x71,0xF4,0x41,0x20,//���ĸ�                                      149-153 
			                       0x01,0x00,0x00,0x00,0x20//ʱ�� *******************************int        154-158
												};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart1_init(115200);	//������ڳ�ʼ��������Ϊ115200
	uart2_init(115200);  //IMU���ڳ�ʼ��������Ϊ115200
	uart3_init(115200);	//������ڳ�ʼ��������Ϊ115200
	uart4_init(115200);	//������ڳ�ʼ��������Ϊ115200
	uart5_init(115200);   //ͨѶ���ڳ�ʼ��������9600
	uart6_init(115200);	//������ڳ�ʼ��������Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�  
	KEY_Init();
	TIM3_Int_Init(250-1,8400-1);//��ѯ������������
	TIM4_Int_Init(100-1,8400-1);//  ��ѯ�ٶ�
	//MPU_Init();					//��ʼ��MPU6050											
	IIC_Init();						//IICͨѶ��ʼ��					
	motor_reclear( );//���״̬���
	delay_ms(100);
	motor_init( );//�����ʼ��
	delay_ms(100);
	enable_init( );//���ʹ��
	delay_ms(100);
	start();//������״̬�
	//while(mpu_dmp_init());
	while(1)
	{
      if(USART_RX_STA&0x8000)                             //�ж��Ƿ���ܵ�����һ֡����************��λ��ͨѶ
      {
         //len=USART_RX_STA & 0x3fff;				             //�õ��˴ν��յ������ݳ���                     
                       
	      /***************** *   	  ���Ʋ���  start   	 * ************************/      
			
         valid = ((USART_RX_BUF[0] & 0xff ) == 0xcd ) && (( USART_RX_BUF[1] & 0xff ) == 0xeb )&& (( USART_RX_BUF[2] & 0xff ) == 0xd7 );
        
         if( valid ) //�ж�����֡   ��Ч
         {
            command_speed_scale1=USART_RX_BUF[9];command_speed_scale2=USART_RX_BUF[10];command_speed_scale3=USART_RX_BUF[11];command_speed_scale4=USART_RX_BUF[12];
					 speed1=max_speed*command_speed_scale1/100;speed2=max_speed*command_speed_scale2/100;
					 speed3=max_speed*command_speed_scale3/100;speed4=max_speed*command_speed_scale4/100;
					// speed=500;
					 switch(USART_RX_BUF[5] & 0x0f )   //�ж�״̬ 1�����
            {
               case 15:     //F
                    speed1=speed1;                                                     
               break;
               case 11:      // B
							      speed1=-speed1;                                                            
               break;						
               case 0:  // S
                    speed1=0;
							      //enable_init();
										//start();                                                                                                        
               break;     
               case 8:   
										motor_reclear();
							      motor_init();
										enable_init();
										start();	
               break;							 
            }    
          switch(USART_RX_BUF[6] & 0xff )   //�ж�״̬ 2��
            {
               case 15:     //F
                    speed2=-speed2;                                                   
               break;
               case 11:      // B
							      speed2=speed2;                                                          
               break;						
               case 0:  // S
                     speed1=0;
										//enable_init();
										//start();                                                                                                        
               break;                       
            } 
             switch(USART_RX_BUF[7] & 0x0f )   //�ж�״̬ 3��
            {
               case 15:     //F
                    speed3=-speed3;                                                     
               break;
               case 11:      // B
							      speed3=speed3;                                                            
               break;						
               case 0:  // S
                    speed3=0;
							      //enable_init();
										//start();                                                                                                        
               break;     
               case 8:   
										motor_reclear();
							      motor_init();
										enable_init();
										start();	
               break;							 
            } 
             switch(USART_RX_BUF[8] & 0x0f )   //�ж�״̬ 4��
            {
               case 15:     //F
                    speed4=speed4;                                                     
               break;
               case 11:      // B
							      speed4=-speed4;                                                            
               break;						
               case 0:  // S
                    speed4=0;
							      //enable_init();
										//start();                                                                                                        
               break;     
               case 8:   
										motor_reclear();
							      motor_init();
										enable_init();
										start();	
               break;							 
            } 						
             if((speed1==0)||(speed2==0)||(speed3==0)||(speed4==0))
						 {
							 
							 stop();//enable_init();
						 }
						else
						{
							qianjin(speed1,speed2,speed3,speed4);
						 start(); start(); 
						}
         } // if(valid) ��β     	                                                                                                             
			USART_RX_STA=0;          
       } //�������ݵ�  
   /*****************************���1������&�ٶȷ���**************************************************/      
   	
			 if((USART2_RX_STA&0xC000)==0x8000)                            //�ж��Ƿ���ܵ�����һ֡����
      {  
        len=0;
				len=USART2_RX_STA&0x3fff;				
	      /***************** *   	  ���Ʋ���  start   	 * ************************/      
         valid = ((USART2_RX_BUF[0] & 0xff ) == 0x00 ) && (( USART2_RX_BUF[1] & 0xff ) == 0x04 );
         if( valid ) //�ж�����֡   ��Ч
         { 
					 encoder_1[0]=USART2_RX_BUF[6];                           //��λ
					 encoder_1[1]=USART2_RX_BUF[7];
					 encoder_1[2]=USART2_RX_BUF[8];
					 encoder_1[3]=USART2_RX_BUF[9];  
          		
           omga_1[0]=USART2_RX_BUF[20];                           //��λ
					 omga_1[1]=USART2_RX_BUF[21];
					 omga_1[2]=USART2_RX_BUF[22];
					 omga_1[3]=USART2_RX_BUF[23]; 					 
         } // if(valid) ��β
         /*for( t=0;t<10;t++)
			{           
				USART1->DR=USART2_RX_BUF[t];         //�����ܵ������ݷ�����λ��
				while((USART1->SR&0X40)==0);			  //�ȴ����ͽ���                                   
			} 	*/			 
			USART2_RX_STA=0;          
       } //�������ݵ�   
     /*****************************���2������&�ٶȷ���**********************************************/      
    if((USART3_RX_STA&0xC000)==0x8000)                            //�ж��Ƿ���ܵ�����һ֡����
      { 
       len=0;
				len=USART3_RX_STA&0x3fff;					
	      /***************** *   	  ���Ʋ���  start   	 * ************************/      
         valid = ((USART3_RX_BUF[0] & 0xff ) == 0x00 ) && (( USART3_RX_BUF[1] & 0xff ) == 0x04 );
         if( valid ) //�ж�����֡   ��Ч
         { 
					 encoder_2[0]=USART3_RX_BUF[6];                           //��λ
					 encoder_2[1]=USART3_RX_BUF[7];
					 encoder_2[2]=USART3_RX_BUF[8];
					 encoder_2[3]=USART3_RX_BUF[9];
					 omga_2[0]=USART3_RX_BUF[20];                           //��λ
					 omga_2[1]=USART3_RX_BUF[21];
					 omga_2[2]=USART3_RX_BUF[22];
					 omga_2[3]=USART3_RX_BUF[23]; 
         } // if(valid) ��β   
			USART3_RX_STA=0;          
       } //�������ݵ�  				 
 /*****************************���4������&�ٶȷ���*************************************************/      
   if((USART6_RX_STA&0xC000)==0x8000)                            //�ж��Ƿ���ܵ�����һ֡����
      {     
	      len=0; 
				len=USART6_RX_STA&0x3fff;
				/***************** *   	  ���Ʋ���  start   	 * ************************/      
         valid = ((USART6_RX_BUF[0] & 0xff ) == 0x00 ) && (( USART6_RX_BUF[1] & 0xff ) == 0x04 );
         if( valid ) //�ж�����֡   ��Ч
         { 
					 encoder_4[0]=USART6_RX_BUF[6];                           //��λ
					 encoder_4[1]=USART6_RX_BUF[7];
					 encoder_4[2]=USART6_RX_BUF[8];
					 encoder_4[3]=USART6_RX_BUF[9];
					 omga_4[0]=USART6_RX_BUF[20];                           //��λ
					 omga_4[1]=USART6_RX_BUF[21];
					 omga_4[2]=USART6_RX_BUF[22];
					 omga_4[3]=USART6_RX_BUF[23]; 
//					 omga_l[0]=USART6_RX_BUF[6];                           //��λ
//					 omga_l[1]=USART6_RX_BUF[7];
//					 omga_l[2]=USART6_RX_BUF[8];
//					 omga_l[3]=USART6_RX_BUF[9];
         } // if(valid) ��β    
			USART6_RX_STA=0;          
       } //�������ݵ�  				 
     /****************************���3������&�ٶȷ���*************************************************/      
    if((UART4_RX_STA&0xC000)==0x8000)                           //�ж��Ƿ���ܵ�����һ֡����
      {   
        len=0; 
				len=UART4_RX_STA&0x3fff;				
	      /***************** *   	  ���Ʋ���  start   	 * ************************/      
         valid = ((UART4_RX_BUF[0] & 0xff ) == 0x00 ) && (( UART4_RX_BUF[1] & 0xff ) == 0x04 );
         if( valid ) //�ж�����֡   ��Ч
         { 
					 encoder_3[0]=UART4_RX_BUF[6];                           //��λ
					 encoder_3[1]=UART4_RX_BUF[7];
					 encoder_3[2]=UART4_RX_BUF[8];
					 encoder_3[3]=UART4_RX_BUF[9];
					 omga_3[0]=UART4_RX_BUF[20];                           //��λ
					 omga_3[1]=UART4_RX_BUF[21];
					 omga_3[2]=UART4_RX_BUF[22];
					 omga_3[3]=UART4_RX_BUF[23]; 
//					 omga_r[0]=UART4_RX_BUF[6];                           //��λ
//					 omga_r[1]=UART4_RX_BUF[7];
//					 omga_r[2]=UART4_RX_BUF[8];
//					 omga_r[3]=UART4_RX_BUF[9];
         } // if(valid) ��β     
			UART4_RX_STA=0;          
       } //�������ݵ�           
	//one encorder
	encorder_one=0;
	encorder_one+=encoder_1[3];
	encorder_one=(encorder_one<<8)+encoder_1[2];
	encorder_one=(encorder_one<<8)+encoder_1[1];
	encorder_one=(encorder_one<<8)+encoder_1[0];
	//two encorder
	encorder_two=0;
	encorder_two+=encoder_2[3];
	encorder_two=(encorder_two<<8)+encoder_2[2];
	encorder_two=(encorder_two<<8)+encoder_2[1];
	encorder_two=(encorder_two<<8)+encoder_2[0];
			 //three encorder
	encorder_three=0;
	encorder_three+=encoder_3[3];
	encorder_three=(encorder_three<<8)+encoder_3[2];
	encorder_three=(encorder_three<<8)+encoder_3[1];
	encorder_three=(encorder_three<<8)+encoder_3[0];
	//four encorder
	encorder_four=0;
	encorder_four+=encoder_4[3];
	encorder_four=(encorder_four<<8)+encoder_4[2];
	encorder_four=(encorder_four<<8)+encoder_4[1];
	encorder_four=(encorder_four<<8)+encoder_4[0];
	//left omga
	omga_left=0;
	omga_left+=omga_l[3];
	omga_left=(omga_left<<8)+omga_l[2];
	omga_left=(omga_left<<8)+omga_l[1];
	omga_left=(omga_left<<8)+omga_l[0];
	//right omga
	omga_right=0;
  omga_right+=omga_r[3];
	omga_right=(omga_right<<8)+omga_r[2];
	omga_right=(omga_right<<8)+omga_r[1];
	omga_right=(omga_right<<8)+omga_r[0];
	
  omga_left=(u32)(omga_left*50556/1517);
	omga_right=(u32)(omga_right*50556/1517);
	omga_right=-omga_right;
	
	omga_one=0;
	omga_one+=omga_1[3];
	omga_one=(omga_one<<8)+omga_1[2];
	omga_one=(omga_one<<8)+omga_1[1];
	omga_one=(omga_one<<8)+omga_1[0];
	
	omga_two=0;
	omga_two+=omga_2[3];
	omga_two=(omga_two<<8)+omga_2[2];
	omga_two=(omga_two<<8)+omga_2[1];
	omga_two=(omga_two<<8)+omga_2[0];
	
	omga_three=0;
	omga_three+=omga_3[3];
	omga_three=(omga_three<<8)+omga_3[2];
	omga_three=(omga_three<<8)+omga_3[1];
	omga_three=(omga_three<<8)+omga_3[0];
	
	omga_four=0;
	omga_four+=omga_4[3];
	omga_four=(omga_four<<8)+omga_4[2];
	omga_four=(omga_four<<8)+omga_4[1];
	omga_four=(omga_four<<8)+omga_4[0];
	//delay_ms(1000);
	//printf("\r\n1:%d\r\n2:%d\r\n3;%d\r\n4:%d\r\n",omga_one,omga_two,omga_three,omga_four);			
   
	//printf("\r\n�Ƕ�:%f\r\n��������:%d\r\n��������;%d\r\n��������:%d\r\n���ٶ�:%d\r\n���ٶ�:%d\r\n",theta,encorder_delta_right,encorder_delta_left,encorder_delta_car,omga_right,omga_left);			
   //theta_car=*(int*)&theta;
	 
	 //MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
	 //MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
	 //if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)theta_car=*(int*)&yaw;
	 delay_ms(30);
	 IMU_Get_Data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz,&pitch,&roll,&yaw,&hx,&hy,&hz);
		 
	 //range=detect(0xDE,0xB0);
	 //printf("\r\n%d\r\n",range);
	//	printf("\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n%d\r\n",aacx,aacx,aacz,gyrox,gyroy,gyroz);
	 //printf("\r\n%f\r\n",yaw);
   x_angular_velocity=gyrox*2000.0/32768;
		 y_angular_velocity=gyroy*2000.0/32768;
		z_angular_velocity=gyroz*2000.0/32768;
		
		x_linear_acceleration=aacx*156.8/32768;
		y_linear_acceleration=aacy*156.8/32768;
		z_linear_acceleration=aacz*156.8/32768;
		
		x_pitch=pitch*180.0/32768;
		y_roll=roll*180.0/32768;
		z_yaw=yaw*180.0/32768;
		
		x_magnetic=hx*1.0;
		y_magnetic=hy*1.0;
		z_magnetic=hz*1.0;
		 
		if(z_yaw<0)z_yaw=360.0+z_yaw;
		
		//printf("\r\n%f\r\n%f\r\n%f\r\n%f\r\n%f\r\n%f\r\n",x_pitch,y_roll,z_yaw,x_angular_velocity,y_angular_velocity,z_angular_velocity);
		xv=*(int*)&x_linear_acceleration;	
		yv=*(int*)&y_linear_acceleration;
		zv=*(int*)&z_linear_acceleration;
		
		xa=*(int*)&x_angular_velocity;
		ya=*(int*)&y_angular_velocity;
		za=*(int*)&z_angular_velocity;
		
		xm=*(int*)&x_pitch;
		ym=*(int*)&y_roll;
		zm=*(int*)&z_yaw;
		
		theta_car=*(int*)&z_yaw;
		
  statuspublish[14]=statuspublish[15]=statuspublish[16]=statuspublish[17]=0xFF;
	statuspublish[14]&=theta_car;
	statuspublish[15]&=theta_car>>8;
	statuspublish[16]&=theta_car>>16;
	statuspublish[17]&=theta_car>>24;
	
	 statuspublish[19]=statuspublish[20]=statuspublish[21]=statuspublish[22]=0xFF;
	statuspublish[19]&=encorder_one;
	statuspublish[20]&=encorder_one>>8;
	statuspublish[21]&=encorder_one>>16;
	statuspublish[22]&=encorder_one>>24;

	statuspublish[24]=statuspublish[25]=statuspublish[26]=statuspublish[27]=0xFF;
	statuspublish[24]&=encorder_two;
	statuspublish[25]&=encorder_two>>8;
	statuspublish[26]&=encorder_two>>16;
	statuspublish[27]&=encorder_two>>24;
	
	statuspublish[29]=statuspublish[30]=statuspublish[31]=statuspublish[32]=0xFF;
	statuspublish[29]&=encorder_three;
	statuspublish[30]&=encorder_three>>8;
	statuspublish[31]&=encorder_three>>16;
	statuspublish[32]&=encorder_three>>24;
	
	statuspublish[34]=statuspublish[35]=statuspublish[36]=statuspublish[37]=0xFF;
  statuspublish[34]&=encorder_four;
	statuspublish[35]&=encorder_four>>8;
	statuspublish[36]&=encorder_four>>16;
	statuspublish[37]&=encorder_four>>24;
	
	statuspublish[39]=statuspublish[40]=statuspublish[41]=statuspublish[42]=0xFF;
	statuspublish[39]&=omga_right;
	statuspublish[40]&=omga_right>>8;
	statuspublish[41]&=omga_right>>16;
	statuspublish[42]&=omga_right>>24;
	
	statuspublish[44]=statuspublish[45]=statuspublish[46]=statuspublish[47]=0xFF;
	statuspublish[44]&=omga_left;
	statuspublish[45]&=omga_left>>8;
	statuspublish[46]&=omga_left>>16;
	statuspublish[47]&=omga_left>>24;
	
	//�ٶ�
	statuspublish[49]=statuspublish[50]=statuspublish[51]=statuspublish[52]=0xFF;
	statuspublish[49]&=omga_one;
	statuspublish[50]&=omga_one>>8;
	statuspublish[51]&=omga_one>>16;
	statuspublish[52]&=omga_one>>24;
	
	statuspublish[54]=statuspublish[55]=statuspublish[56]=statuspublish[57]=0xFF;
  statuspublish[54]&=omga_two;
	statuspublish[55]&=omga_two>>8;
	statuspublish[56]&=omga_two>>16;
	statuspublish[57]&=omga_two>>24;
	
	statuspublish[59]=statuspublish[60]=statuspublish[61]=statuspublish[62]=0xFF;
	statuspublish[59]&=omga_three;
	statuspublish[60]&=omga_three>>8;
	statuspublish[61]&=omga_three>>16;
	statuspublish[62]&=omga_three>>24;
	
	statuspublish[64]=statuspublish[65]=statuspublish[66]=statuspublish[67]=0xFF;
	statuspublish[64]&=omga_four;
	statuspublish[65]&=omga_four>>8;
	statuspublish[66]&=omga_four>>16;
	statuspublish[67]&=omga_four>>24;
	
	statuspublish[69]=statuspublish[70]=statuspublish[71]=statuspublish[72]=0xFF;
  statuspublish[69]&=xa;
	statuspublish[70]&=xa>>8;
	statuspublish[71]&=xa>>16;
	statuspublish[72]&=xa>>24;
	
	statuspublish[74]=statuspublish[75]=statuspublish[76]=statuspublish[77]=0xFF;
  statuspublish[74]&=ya;
	statuspublish[75]&=ya>>8;
	statuspublish[76]&=ya>>16;
	statuspublish[77]&=ya>>24;
	
	statuspublish[79]=statuspublish[80]=statuspublish[81]=statuspublish[82]=0xFF;
  statuspublish[79]&=za;
	statuspublish[80]&=za>>8;
	statuspublish[81]&=za>>16;
	statuspublish[82]&=za>>24;

  statuspublish[84]=statuspublish[85]=statuspublish[86]=statuspublish[87]=0xFF;
  statuspublish[84]&=xv;
	statuspublish[85]&=xv>>8;
	statuspublish[86]&=xv>>16;
	statuspublish[87]&=xv>>24;
	
	statuspublish[89]=statuspublish[90]=statuspublish[91]=statuspublish[92]=0xFF;
  statuspublish[89]&=yv;
	statuspublish[90]&=yv>>8;
	statuspublish[91]&=yv>>16;
	statuspublish[92]&=yv>>24;
	
	statuspublish[94]=statuspublish[95]=statuspublish[96]=statuspublish[97]=0xFF;
  statuspublish[94]&=zv;
	statuspublish[95]&=zv>>8;
	statuspublish[96]&=zv>>16;
	statuspublish[97]&=zv>>24;
	
	statuspublish[99]=statuspublish[100]=statuspublish[101]=statuspublish[102]=0xFF;
  statuspublish[99]&=xm;
	statuspublish[100]&=xm>>8;
	statuspublish[101]&=xm>>16;
	statuspublish[102]&=xm>>24;
	
	statuspublish[104]=statuspublish[105]=statuspublish[106]=statuspublish[107]=0xFF;
  statuspublish[104]&=ym;
	statuspublish[105]&=ym>>8;
	statuspublish[106]&=ym>>16;
	statuspublish[107]&=ym>>24;
	
	statuspublish[109]=statuspublish[110]=statuspublish[111]=statuspublish[112]=0xFF;
  statuspublish[109]&=zm;
	statuspublish[110]&=zm>>8;
	statuspublish[111]&=zm>>16;
	statuspublish[112]&=zm>>24;
				/****************************����״̬д��************************************/

  //printf("\r\n1:%d\r\n2:%d\r\n3;%d\r\n4:%d\r\n",aacx,aacy,aacz,xa);			
   
//delay_ms(1000);
//printf("\r\n�Ƕ�:%f\r\n��������:%d\r\n��������;%d\r\n��������:%d\r\n���ٶ�:%d\r\n���ٶ�:%d\r\n",theta,encorder_right,encorder_left,encorder_delta_car,omga_right,omga_left);			



				 
      /*********************************��ѯ********************************************/
			if(flag_50ms==1)
				 {
				 chaxun_encoder();
				delay_ms(1);
					// chaxun_velocity();
					 flag_50ms=0;
				 }
			
			 

				 
			 /*********************************���͵���״̬*************************************/
	if(flag_500ms==1)
			 {

				 for( t=0;t<159;t++)
			{           
				USART1->DR=statuspublish[t];        
				while((USART1->SR&0X40)==0);			                                    
			}     				        
        flag_500ms=0;      
		  }
			 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)
		{
			motor_reclear();
			motor_init();
			enable_init();
			start();
		}
		else if(key==KEY0_PRES)
		{
			z_zero();
		}
		else if(key==KEY1_PRES)
		{
			chaxun_velocity();
		}
		else if(key==KEY2_PRES)
		{
enable_init();
		}
		else
		{
			//times++;
		//	if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			  
		}
	}

}

