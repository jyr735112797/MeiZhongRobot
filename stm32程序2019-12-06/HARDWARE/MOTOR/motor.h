#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
#include "usart.h"
//���������غ�������
 

void motor_reclear(void);   //���״̬��λ
void motor_init(void);      //���״̬��ʼ��
void enable_init(void);     //���ʹ�ܺ���
void start(void);           //�����������
void qianjin(u32 speed11,u32 speed22,u32 speed33,u32 speed44);  //����ٶȿ��ƺ���
void chaxun_velocity(void);    //�����ǰʱ���ٶȲ�ѯ����
void chaxun_encoder(void);     //�����ǰʱ��λ�ò�ѯ
void stop(void);               //���ֹͣ�����
u16 crc_calculation(u8 dianji[]);
void houtui(u32 speedl,u32 speedr);
void zuozhuan(u32 speedl,u32 speedr);
void youzhuan(u32 speedl,u32 speedr);
void zuoyi(void);
void youyi(void);
void cuowu(void);

#endif
