#include "ros/ros.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>

void getrvizgoal(const geometry_msgs::PoseStamped::ConstPtr& goal)
{
    move_base_msgs::MoveBaseActionGoal action_goal;
    action_goal.header.stamp = ros::Time::now();
    action_goal.goal.target_pose = *goal;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rviz_nav_goal");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe<geometry_msgs::PoseStamped>("goal", 1, getrvizgoal);

  ros::spin();

  return 0;
}
