#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include<iostream>
using namespace std;

class Teleop
{
public:
    Teleop();

private:
    /* data */
    void callback(const sensor_msgs::Joy::ConstPtr& Joy);
    ros::NodeHandle n; //实例化节点
    ros::Subscriber sub ;
    ros::Publisher pub ;
    double vlinear,vangular;//我们控制乌龟的速度，是通过这两个变量调整
    int axis_ang,axis_lin,ton;	//axes[]的键
};

Teleop::Teleop()
{
//我们将这几个变量加上参数，可以在参数服务器方便修改
 n.param<int>("axis_linear",axis_lin,1); //默认axes[1]接收速度
n.param<int>("axis_angular",axis_ang,0);//默认axes[0]接收角度
n.param<double>("vel_linear",vlinear,1);//默认线速度1 m/s
n.param<double>("vel_angular",vangular,1);//默认角速度1 单位rad/s
n.param<int>("button",ton,1);//默认角速度1 单位rad/s
 pub = n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);//将速度发给乌龟
ros::Rate loop_rate(10);
sub = n.subscribe<sensor_msgs::Joy>("joy",10,&Teleop::callback,this); //订阅游戏手柄发来的数据}
}
void Teleop::callback(const sensor_msgs::Joy::ConstPtr& Joy)

{
geometry_msgs::Twist v;
v.linear.x =Joy->axes[axis_lin]*vlinear; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
 v.angular.z =Joy->axes[axis_ang]*vangular;

/*
 * if(Joy->buttons[ton])
{
    v.linear.x =0; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
     v.angular.z =0;

}*/





/*
 * if(Joy->axes[1]==1)
{
v.linear.x =1; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
v.angular.z=0;
}
else if(Joy->axes[1]==-1)
{
v.linear.x =-1; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
v.angular.z=0;
}
else if(Joy->axes[0]==1)
{
v.linear.x =0;
v.angular.z=-1; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
}
else if(Joy->axes[0]==-1)
{
v.linear.x =0;
 v.angular.z=1; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
}
else if(Joy->buttons[1]==1)
{
v.linear.x =0; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
v.angular.z =0;
}
*/
//{
//v.linear.x =Joy->axes[axis_lin]*vlinear; //将游戏手柄的数据乘以你想要的速度，然后发给乌龟
 //v.angular.z =Joy->axes[axis_ang]*vangular;
//}
 ROS_INFO("linear:%.3lf angular:%.3lf",v.linear.x,v.angular.z);
pub.publish(v);
ros::Rate loop_rate(10);
}

int main(int argc,char** argv)

{
 ros::init(argc, argv, "logteleop");
 Teleop telelog; ros::spin();
 return 0;
}

