#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
#include "usart.h"
//电机操作相关函数定义
 

void motor_reclear(void);   //电机状态复位
void motor_init(void);      //电机状态初始化
void enable_init(void);     //电机使能函数
void start(void);           //电机启动函数
void qianjin(u32 speed11,u32 speed22,u32 speed33,u32 speed44);  //电机速度控制函数
void chaxun_velocity(void);    //电机当前时刻速度查询函数
void chaxun_encoder(void);     //电机当前时刻位置查询
void stop(void);               //电机停止命令函数
u16 crc_calculation(u8 dianji[]);
void houtui(u32 speedl,u32 speedr);
void zuozhuan(u32 speedl,u32 speedr);
void youzhuan(u32 speedl,u32 speedr);
void zuoyi(void);
void youyi(void);
void cuowu(void);

#endif
