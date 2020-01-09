#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <JY901.h>
#include <sensor_msgs/Imu.h>
#include <sstream>
#include "geometry_msgs/Pose2D.h"
serial::Serial ser; //声明串口对象
//回调函数
void write_callback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO_STREAM("Writing to serial port" <<msg->data);
    ser.write(msg->data);   //发送串口数据
}

int main(int argc, char **argv)

{      //初始化节点
    ros::init(argc, argv, "serial_imu_node");
    //声明节点句柄
    ros::NodeHandle nh;
    //订阅主题，并配置回调函数
    ros::Subscriber IMU_write_pub = nh.subscribe("imu_command", 1000,write_callback);
    //发布主题, 消息格式使用sensor_msg::Imu标准格式（topic名称，队列长度）
    ros::Publisher IMU_read_pub = nh.advertise<sensor_msgs::Imu>("imu_data_raw", 1000);
    ros::Publisher mPose2DPub = nh.advertise<geometry_msgs::Pose2D>("xqserial_server/Pose2D",1000,true);
    //打开串口
    try
    {
    //设置串口属性，并打开串口
        ser.setPort("/dev/ttyUSB1");
        ser.setBaudrate(9600);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }
    //检测串口是否已经打开，并给出提示信息
    if(ser.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        return -1;
    }
    //消息发布频率
    ros::Rate loop_rate(1000);
    while (ros::ok()){
      //处理从串口来的Imu数据
        //串口缓存字符数
         unsigned short  data_size=50;
        //if(data_size = ser.available()){
            //ser.available(当串口没有缓存时，这个函数会一直等到有缓存才返回字符数
          
	char  tmpdata[data_size];
            ser.read((unsigned char *)tmpdata, data_size);
           JY901.CopeSerialData(tmpdata,data_size);
           //JY901 imu 库函数
           //打包IMU数据
            sensor_msgs::Imu imu_data;

geometry_msgs::Pose2D CarPos2D;

           imu_data.header.stamp = ros::Time::now();
            imu_data.header.frame_id = "base_link";
            imu_data.orientation.x = (double)JY901.stcAngle.Angle[0]/32768*180;
            imu_data.orientation.y = (double)JY901.stcAngle.Angle[1]/32768*180;
           imu_data.orientation.z = (double)JY901.stcAngle.Angle[2]/32768*180;

           imu_data.angular_velocity.x = (double)JY901.stcGyro.w[0]/32768*2000;
           imu_data.angular_velocity.y = (double)JY901.stcGyro.w[1]/32768*2000;
           imu_data.angular_velocity.z = (double)JY901.stcGyro.w[2]/32768*2000;

           imu_data.linear_acceleration.x = (double)JY901.stcAcc.a[0]/32768*156.8;
           imu_data.linear_acceleration.y = (double)JY901.stcAcc.a[1]/32768*156.8;
           imu_data.linear_acceleration.z = (double)JY901.stcAcc.a[2]/32768*156.8;   
CarPos2D.theta=imu_data.orientation.z;
           //发布topic
           IMU_read_pub.publish(imu_data);
mPose2DPub.publish(CarPos2D);
        //}
        //处理ROS的信息，比如订阅消息,并调用回调函数
        ros::spinOnce();
        loop_rate.sleep();
  }
ser.close();      
return 0;
 }

