#ifndef _IMU_H
#define _IMU_H
#include "sys.h"


u8 IMU_Get_Data(short *ax,short *ay,short *az,short *gx,short *gy,short *gz,short *pitch,short *roll,short *yaw,short *hx,short *hy,short *hz);  
	void z_zero( );
#endif
