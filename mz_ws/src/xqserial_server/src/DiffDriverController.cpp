#include "DiffDriverController.h"
#include <time.h>

namespace xqserial_server
{


DiffDriverController::DiffDriverController()
{
    max_wheelspeed=2.0;
    cmd_topic="cmd_vel";
    xq_status=new StatusPublisher();
    cmd_serial=NULL;
    MoveFlag=true;
}

DiffDriverController::DiffDriverController(double max_speed_,std::string cmd_topic_,StatusPublisher* xq_status_,CallbackAsyncSerial* cmd_serial_)
{
    MoveFlag=true;
    max_wheelspeed=max_speed_;
    cmd_topic=cmd_topic_;
    xq_status=xq_status_;
    cmd_serial=cmd_serial_;
}

void DiffDriverController::run()
{
    ros::NodeHandle nodeHandler;
    ros::Subscriber sub = nodeHandler.subscribe(cmd_topic, 1, &DiffDriverController::sendcmd, this);
    ros::Subscriber sub2 = nodeHandler.subscribe("/imu_cal", 1, &DiffDriverController::imuCalibration,this);
    ros::Subscriber sub3 = nodeHandler.subscribe("/globalMoveFlag", 1, &DiffDriverController::updateMoveFlag,this);
    ros::Subscriber sub4 = nodeHandler.subscribe("/barDetectFlag", 1, &DiffDriverController::updateBarDetectFlag,this);
    ros::spin();
}
void DiffDriverController::updateMoveFlag(const std_msgs::Bool& moveFlag)
{
  boost::mutex::scoped_lock lock(mMutex);
  MoveFlag=moveFlag.data;

}
void DiffDriverController::imuCalibration(const std_msgs::Bool& calFlag)
{
  if(calFlag.data)
  {
    //下发底层ｉｍｕ标定命令
    char cmd_str[5]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x01,(char)0x43};
    if(NULL!=cmd_serial)
    {
        cmd_serial->write(cmd_str,5);
    }
  }
}
void DiffDriverController::updateBarDetectFlag(const std_msgs::Bool& DetectFlag)
{
  if(DetectFlag.data)
  {
    //下发底层红外开启命令
    char cmd_str[6]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x02,(char)0x44,(char)0x01};
    if(NULL!=cmd_serial)
    {
        cmd_serial->write(cmd_str,6);
    }
  }
  else
  {
    //下发底层红外禁用命令
    char cmd_str[6]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x02,(char)0x44,(char)0x00};
    if(NULL!=cmd_serial)
    {
        cmd_serial->write(cmd_str,6);
    }
  }
}
void DiffDriverController::sendcmd(const geometry_msgs::Twist &command)
{
    static time_t t1=time(NULL),t2;
    int i=0,wheel_ppr=1;
    double separation=0,radius=0,speed_lin_x=0,speed_lin_y=0,speed_ang=0,speed_temp[4],w1=0,w2=0,w3=0,w4=0;
    char speed[4]={0,0,0,0};//右一左二
   // char cmd_str[13]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x09,(char)0x74,(char)0x53,(char)0x53,(char)0x53,(char)0x53,(char)0x00,(char)0x00,(char)0x00,(char)0x00};
  // char cmd_str[8]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x02,(char)0x74,(char)0x00,(char)0x0d,(char)0x0a};
   // char cmd_str[11]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x02,(char)0x74,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x0d,(char)0x0a};
     char cmd_str[15]={(char)0xcd,(char)0xeb,(char)0xd7,(char)0x02,(char)0x74,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x00,(char)0x0d,(char)0x0a};
    if(xq_status->get_status()==0) return;//底层还在初始化
    separation=xq_status->get_wheel_separation();
    radius=xq_status->get_wheel_radius();
    wheel_ppr=50556;//xq_status->get_wheel_ppr();
    //转换速度单位，由米转换成转
    //speed_lin=command.linear.x/(2.0*PI*radius);
    //speed_ang=command.angular.z*separation/(2.0*PI*radius);
    //JYR
    speed_lin_y=command.linear.x;
    speed_lin_x=-command.linear.y;
    speed_ang=command.angular.z;

    w1=(speed_lin_y+speed_lin_x)/(2.0*PI*0.0762)-speed_ang*700.0/(4.0*PI*76.2);
    w2=(speed_lin_y-speed_lin_x)/(2.0*PI*0.0762)+speed_ang*700.0/(4.0*PI*76.2);
    w3=(speed_lin_y+speed_lin_x)/(2.0*PI*0.0762)+speed_ang*700.0/(4.0*PI*76.2);
    w4=(speed_lin_y-speed_lin_x)/(2.0*PI*0.0762)-speed_ang*700.0/(4.0*PI*76.2);
    //float scale=std::max(std::abs(speed_lin+speed_ang/2.0),std::abs(speed_lin-speed_ang/2.0))/max_wheelspeed;
    //JYR
    float scale=std::max(std::max(std::abs(w1),std::abs(w3)),std::max(std::abs(w2),std::abs(w4)))/max_wheelspeed;

    if(scale>1.0)
    {
      scale=1.0/scale;
    }
    else
    {
      scale=1.0;
    }
    //转出最大速度百分比,并进行限幅
    speed_temp[0]=scale*w1/max_wheelspeed*100.0;
    speed_temp[0]=std::min(speed_temp[0],100.0);
    speed_temp[0]=std::max(-100.0,speed_temp[0]);

    speed_temp[1]=scale*w2/max_wheelspeed*100.0;
    speed_temp[1]=std::min(speed_temp[1],100.0);
    speed_temp[1]=std::max(-100.0,speed_temp[1]);

    speed_temp[2]=scale*w3/max_wheelspeed*100.0;
    speed_temp[2]=std::min(speed_temp[2],100.0);
    speed_temp[2]=std::max(-100.0,speed_temp[2]);

    speed_temp[3]=scale*w4/max_wheelspeed*100.0;
    speed_temp[3]=std::min(speed_temp[3],100.0);
    speed_temp[3]=std::max(-100.0,speed_temp[3]);

  //std::cout<<"radius "<<radius<<std::endl;
  //std::cout<<"ppr "<<wheel_ppr<<std::endl;
  //std::cout<<"pwm "<<speed_temp[0]<<std::endl;
  //  command.linear.x/
  /*  for(i=0;i<2;i++)
    {
     speed[i]=(int8_t)speed_temp[i];
     if(speed[i]<0)
     {
         cmd_str[5+i]=(char)0x42;//B
         cmd_str[9+i]=-speed[i];
     }
     else if(speed[i]>0)
     {
         cmd_str[5+i]=(char)0x46;//F
         cmd_str[9+i]=speed[i];
     }
     else
     {
         cmd_str[5+i]=(char)0x53;//S
         cmd_str[9+i]=(char)0x00;
     }
  }
*/
    for(i=0;i<4;i++)//右一左二
        {
         speed[i]=(int8_t)speed_temp[i];
         if(speed[i]<0)
         {
             cmd_str[5+i]=(char)0x0b;//B
             cmd_str[9+i]=-speed[i];
         }
         else if(speed[i]>0)
         {
             cmd_str[5+i]=(char)0x0f;//F
             cmd_str[9+i]=speed[i];
         }
         else
         {
             cmd_str[5+i]=(char)0x00;//S
             cmd_str[9+i]=(char)0x00;
         }
      }

    /*for(i=0;i<2;i++)
        {
         speed[i]=(int8_t)speed_temp[i];
    }
         if((speed[0]<0)&&(speed[1]<0))
         {
             cmd_str[5]=(char)0x02;//B
             cmd_str[6]=abs(speed[2]);//L
             cmd_str[7]=abs(speed[1]);//R
         }
         else if((speed[0]>0)&&(speed[1]>0))
         {
             cmd_str[5]=(char)0x01;//F
             cmd_str[6]=abs(speed[2]);//L
             cmd_str[7]=abs(speed[1]);//R
         }
         else if((speed[0]<0)&&(speed[1]>0))
         {
             cmd_str[5]=(char)0x03;//L
             cmd_str[6]=abs(speed[2]);//L
             cmd_str[7]=abs(speed[1]);//R
         }
         else if((speed[0]>0)&&(speed[1]<0))
         {
             cmd_str[5]=(char)0x04;//R
             cmd_str[6]=abs(speed[2]);//L
             cmd_str[7]=abs(speed[1]);//R
         }
         else
         {
             cmd_str[5]=(char)0x00;//S
             cmd_str[6]=0;//L
             cmd_str[7]=0;//R
         }
*/
    // std::cout<<"distance1 "<<xq_status->car_status.distance1<<std::endl;
    // std::cout<<"distance2 "<<xq_status->car_status.distance2<<std::endl;
    // std::cout<<"distance3 "<<xq_status->car_status.distance3<<std::endl;
    // if(xq_status->get_status()==2)
    // {
    //   //有障碍物
    //   if(xq_status->car_status.distance1<30&&xq_status->car_status.distance1>0&&cmd_str[6]==(char)0x46)
    //   {
    //     cmd_str[6]=(char)0x53;
    //   }
    //   if(xq_status->car_status.distance2<30&&xq_status->car_status.distance2>0&&cmd_str[5]==(char)0x46)
    //   {
    //     cmd_str[5]=(char)0x53;
    //   }
    //   if(xq_status->car_status.distance3<20&&xq_status->car_status.distance3>0&&(cmd_str[5]==(char)0x42||cmd_str[6]==(char)0x42))
    //   {
    //     cmd_str[5]=(char)0x53;
    //     cmd_str[6]=(char)0x53;
    //   }
    //   if(xq_status->car_status.distance1<15&&xq_status->car_status.distance1>0&&(cmd_str[5]==(char)0x46||cmd_str[6]==(char)0x46))
    //   {
    //     cmd_str[5]=(char)0x53;
    //     cmd_str[6]=(char)0x53;
    //   }
    //   if(xq_status->car_status.distance2<15&&xq_status->car_status.distance2>0&&(cmd_str[5]==(char)0x46||cmd_str[6]==(char)0x46))
    //   {
    //     cmd_str[5]=(char)0x53;
    //     cmd_str[6]=(char)0x53;
    //   }
    // }

  boost::mutex::scoped_lock lock(mMutex);
   /* if(!MoveFlag)
    {
      cmd_str[5]=(char)0x53;
      cmd_str[6]=(char)0x53;
    }
    */
    if(NULL!=cmd_serial)
    {
        cmd_serial->write(cmd_str,15);
    }

   // command.linear.x
}






















}
