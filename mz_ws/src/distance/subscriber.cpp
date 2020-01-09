#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
class Listener
{
public:
 //std::string copy_data = "init init init";
  int count = 0;
  int flag = 0;
public:
  void callback(const std_msgs::Int8& msg);
  void getrvizgoal(const geometry_msgs::PoseStamped::ConstPtr& goal);

};
void Listener::callback(const std_msgs::Int8& msg)
{
  ROS_INFO("I heard: [%d]", msg.data);
  //std::stringstream ss;
  //ss << msg->data.c_str();
  //ss >> copy_data;
  //std::cout <<"copy_data is: " << copy_data <<"\n";
  //print_data2();
  count = msg.data;
}
void Listener::getrvizgoal(const geometry_msgs::PoseStamped::ConstPtr& goal)
{
    move_base_msgs::MoveBaseActionGoal rviz_goal_;
    rviz_goal_.header.stamp = ros::Time::now();
    rviz_goal_.goal.target_pose = *goal;
    flag = 1;
    ROS_INFO("I heard: x = [%lf] y = [%lf]", goal->pose.position.x,goal->pose.position.y);
}

/*void chatterCallback(const std_msgs::Int8& msg)
{
  ROS_INFO("I heard: [%d]", msg.data);

}
*/
int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;
  ros::NodeHandle nh1;
  MoveBaseClient ac("move_base", true);
  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "base_link";
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 1.0;
  goal.target_pose.pose.orientation.w = 1.0;

  Listener listener;
  //ros::Subscriber sub = nh.subscribe("int8", 1000, chatterCallback);
  ros::Subscriber sub = nh.subscribe("goal", 1000, &Listener::getrvizgoal, &listener);
  ros::Subscriber sub1 = nh1.subscribe("int8", 1000, &Listener::callback, &listener);
  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    //ROS_INFO("%d", msg.data);
  /* if(listener.count == 40)
   {
     ROS_INFO("Sending goal");
     goal.target_pose.header.frame_id = "base_link";
     goal.target_pose.header.stamp = ros::Time::now();

     goal.target_pose.pose.position.x = 1.0;
     goal.target_pose.pose.orientation.w = 1.0;
     ac.sendGoal(goal);

     ac.waitForResult();
   }*/
  if((listener.flag == 1)&&(listener.count == 40))
  {
    listener.flag =0;
    ROS_INFO("GOT IT");
    sleep(10);
  }
    ros::spinOnce();
    loop_rate.sleep();
  }
  //ros::spin();

  return 0;
}
