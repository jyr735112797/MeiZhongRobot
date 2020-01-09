#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
int main(int argc, char **argv)
{
  ros::init(argc, argv, "publisher");
  ros::NodeHandle nh;

  //ros::Publisher chatter_pub = nh.advertise<std_msgs::Int8>("int8", 1000);
  ros::Publisher chatter_pub = nh.advertise<std_msgs::Int8>("int8", 1000);
  ros::Rate loop_rate(10);
  int count = 0;
  while (ros::ok())
  {
    std_msgs::Int8 msg;
    if(count >50)
    {
      msg.data = 40;
      count = 0;
    }
    else
      msg.data = 20;


    ROS_INFO("%d", msg.data);

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}
