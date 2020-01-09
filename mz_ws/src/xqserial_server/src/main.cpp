
#include "AsyncSerial.h"

#include <iostream>
#include <boost/thread.hpp>

#include <ros/ros.h>
#include <ros/package.h>
#include "DiffDriverController.h"
#include "StatusPublisher.h"

using namespace std;

int main(int argc, char **argv)
{
    cout<<"welcome to xiaoqiang serial server,please feel free at home!"<<endl;

    ros::init(argc, argv, "xqserial_server");
    ros::start();
    
    //获取串口参数
    std::string port;
    ros::param::param<std::string>("~port", port, "/dev/stm32Car");
    int baud;
    ros::param::param<int>("~baud", baud, 115200);
    cout<<"port:"<<port<<" baud:"<<baud<<endl;
    //获取小车机械参数
    double separation=0,radius=0;
    bool DebugFlag = false;
    ros::param::param<double>("~wheel_separation", separation, 0.37);
    ros::param::param<double>("~wheel_radius", radius, 0.0625);
    ros::param::param<bool>("~debug_flag", DebugFlag, false);
    xqserial_server::StatusPublisher xq_status(separation,radius);

    //获取小车控制参数
    double max_speed;
    string cmd_topic;
    ros::param::param<double>("~max_speed", max_speed, 2.0);
    ros::param::param<std::string>("~cmd_topic", cmd_topic, "cmd_vel");

    try {
        CallbackAsyncSerial serial(port,baud);
        serial.setCallback(boost::bind(&xqserial_server::StatusPublisher::Update,&xq_status,_1,_2));
        xqserial_server::DiffDriverController xq_diffdriver(max_speed,cmd_topic,&xq_status,&serial);
        boost::thread cmd2serialThread(& xqserial_server::DiffDriverController::run,&xq_diffdriver);
        // send test flag
        char debugFlagCmd[] = {(char)0xcd, (char)0xeb, (char)0xd7, (char)0x01, 'T',(char)0x0d,(char)0x0a};
        if(DebugFlag){
          std::cout << "Debug mode Enabled" << std::endl;
          serial.write(debugFlagCmd, 7);
        }
        // send reset cmd
        char resetCmd[] = {(char)0xcd, (char)0xeb, (char)0xd7, (char)0x01, 'I',(char)0x0d,(char)0x0a};
        serial.write(resetCmd, 7);
        ros::Duration(0.5).sleep();

        //jyr  send enable
        char enableCmd[] = {(char)0xcd, (char)0xeb, (char)0xd7, (char)0x02, (char)0x74,(char)0x08,(char)0x0d,(char)0x0a};
        serial.write(enableCmd, 8);
       ros::Duration(0.5).sleep();

        ros::Rate r(100);//发布周期为50hz

        while (ros::ok())
        {
            if(serial.errorStatus() || serial.isOpen()==false)
            {
                ROS_ERROR_STREAM("Error: serial port closed unexpectedly");
                break;
            }
            xq_status.Refresh();//定时发布状态
            r.sleep();
            //cout<<"run"<<endl;
        }

        quit:
        serial.close();

    } catch (std::exception& e) {
        ROS_ERROR_STREAM("Open " << port << " failed.");
        ROS_ERROR_STREAM("Exception: " << e.what());
    }

    ros::shutdown();
    return 0;
}
