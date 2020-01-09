#include <ros/ros.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <boost/foreach.hpp>
#define forEach BOOST_FOREACH
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf/tf.h>
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#include "tf/message_filter.h"
#include "std_msgs/Int8.h"
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <global_planner/planner_core.h>
#include <vector>
#include <string>

#include <ros/ros.h>

#include <actionlib/server/simple_action_server.h>
#include <move_base_msgs/MoveBaseAction.h>

#include <nav_core/base_local_planner.h>
#include <nav_core/base_global_planner.h>
#include <nav_core/recovery_behavior.h>
#include <geometry_msgs/PoseStamped.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <costmap_2d/costmap_2d.h>
#include <nav_msgs/GetPlan.h>

#include <pluginlib/class_loader.hpp>
#include <std_srvs/Empty.h>

#include <dynamic_reconfigure/server.h>
#include "move_base/MoveBaseConfig.h"
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>

#include <geometry_msgs/Twist.h>

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "std_msgs/Float32.h"
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class Listener
{
public:
 //std::string copy_data = "init init init";
  float count = 0;
  int flag = 0;
  std::vector<geometry_msgs::PoseStamped> plannnnn_;
public:
  void callback(const std_msgs::Float32& msg);

};

void Listener::callback(const std_msgs::Float32& msg)
{
  ROS_INFO("I heard: [%f]", msg.data);

  count = msg.data;
}
Listener listener;
//std::vector<geometry_msgs::Point> plannnnn_;
bool getbaselinkpose(tf::Stamped<tf::Pose>& odom_pose,double& x, double& y, double& yaw,
                               const ros::Time& t){
  tf::Stamped<tf::Pose> ident (tf::Transform(tf::createIdentityQuaternion(),
  tf::Vector3(0,0,0)), ros::Time::now(), "base_link" );
 tf::StampedTransform transform;
 tf::TransformListener* tf1_;
  try
  {
  tf1_ = new tf::TransformListener();
  //tf1_->transformPose("map", ident, odom_pose);
  tf1_->waitForTransform("map", "base_link",ros::Time(0), ros::Duration(3.0));
  tf1_->lookupTransform("map","base_link",ros::Time(0),transform);
  }
  catch(tf::TransformException e)
  {
  ROS_WARN("Failed to compute odom pose, skipping scan (%s)", e.what());
  return false;
  }

 x = transform.getOrigin().x();
 y = transform.getOrigin().y();
 double pitch,roll;
 odom_pose.getBasis().getEulerYPR(yaw, pitch, roll);

  return true;}


void fillPathRequest(nav_msgs::GetPlan::Request &request)
{
  tf::Stamped<tf::Pose> odom_pose;
  double x, y, yaw;
  ros::Time t=ros::Time::now();
  getbaselinkpose(odom_pose,x,y,yaw,t);
  ROS_INFO("x=%lf,y=%lf",x,y);
request.start.header.frame_id ="map";
request.start.pose.position.x = x;//初始位置x坐标
request.start.pose.position.y = y;//初始位置y坐标
request.start.pose.orientation.w = yaw;//方向
request.goal.header.frame_id = "map";
request.goal.pose.position.x = 22.6;//0.8;//终点坐标
request.goal.pose.position.y = 17.5;//3;
request.goal.pose.orientation.w = 0.7;
request.tolerance = 0.5;//如果不能到达目标，最近可到的约束
}
//路线规划结果回调

void callPlanningService(ros::ServiceClient &serviceClient, nav_msgs::GetPlan &srv)
{
double interval = 3.5;
std::vector<geometry_msgs::PoseStamped> plannn;
      if (serviceClient.call(srv)) {

      if (!srv.response.plan.poses.empty()) {
      //forEach(const geometry_msgs::PoseStamped &p, srv.response.plan.poses)
        for(size_t i = 1; i < (srv.response.plan.poses.size()); i++)
        {

      ROS_INFO("x = %f, y = %f   w=%lf", srv.response.plan.poses[i].pose.position.x, srv.response.plan.poses[i].pose.position.y,srv.response.plan.poses[i].pose.orientation.w);
      plannn.push_back(srv.response.plan.poses[i]);

      }
        for(size_t i = 1; i < (plannn.size()); i++)
        {

        ROS_INFO("planx = %f, y = %f",plannn[i].pose.position.x ,plannn[i].pose.position.y);

        }

        if(plannn.size() < 3)
        {
            listener.plannnnn_.push_back(plannn[0]);
            listener.plannnnn_.push_back(plannn[1]);
           // w.push_back(start_pose.pose.orientation);
           // w.push_back(goal_pose.pose.orientation);
        }
        else
        {
            geometry_msgs::PoseStamped last_pose = plannn[0];
            //geometry_msgs::Quaternion last_wwww = plannn[0].pose.orientation;
             listener.plannnnn_.push_back(last_pose);
            //w.push_back(last_wwww);
            for(size_t i = 1; i < (plannn.size()-1); i++)
            {
                geometry_msgs::PoseStamped pose = plannn[i];
                //geometry_msgs::Quaternion wwww = global_plan[i].pose.orientation;
                double dis = hypot(last_pose.pose.position.x-pose.pose.position.x, last_pose.pose.position.y-pose.pose.position.y);
                if(dis >= interval)
                {
                    last_pose = pose;
                    listener.plannnnn_.push_back(last_pose);
                    //w.push_back(last_wwww);
                    ROS_INFO("interval x=%f,y=%f",last_pose.pose.position.x,last_pose.pose.position.y);
                }

            }
            listener.plannnnn_.push_back(plannn[plannn.size()-1]);
            //w.push_back(global_plan[global_plan.size()-1].pose.orientation);
        }
      }
      else {
      ROS_WARN("Got empty plan");
      }
      }
      else {
       ROS_ERROR("Failed to call service %s - is the robot moving?",
      serviceClient.getService().c_str());
      }
}


int main(int argc, char** argv)
{
 // Listener listener;

  //std::vector<geometry_msgs::Point> plannnnn_;
ros::init(argc, argv, "fun_node");
ros::NodeHandle nh;
std::string service_name = "move_base/make_plan";
size_t i =1;
int a =0;


while (!ros::service::waitForService(service_name, ros::Duration(3.0))) {
ROS_INFO("Waiting for service move_base/make_plan to become available");
}

ros::ServiceClient serviceClient = nh.serviceClient<nav_msgs::GetPlan>(service_name, true);
if (!serviceClient) {
ROS_FATAL("Could not initialize get plan service from %s",
serviceClient.getService().c_str());
return -1;
}
nav_msgs::GetPlan srv;

fillPathRequest(srv.request);
if (!serviceClient) {
ROS_FATAL("Persistent service connection to %s failed",
serviceClient.getService().c_str());
return -1;
}
ROS_INFO("conntect to %s",serviceClient.getService().c_str());
callPlanningService(serviceClient, srv);
std::vector<geometry_msgs::Quaternion> quaternions;
geometry_msgs::Quaternion q;
for(size_t i = 1; i < (listener.plannnnn_.size()); i++)
{

double angle;
ROS_INFO("planx = %f, y = %f",listener.plannnnn_[i].pose.position.x ,listener.plannnnn_[i].pose.position.y);
angle = atan((listener.plannnnn_[i].pose.position.y-listener.plannnnn_[i-1].pose.position.y)/(listener.plannnnn_[i].pose.position.x-listener.plannnnn_[i-1].pose.position.x));
if((listener.plannnnn_[i].pose.position.y-listener.plannnnn_[i-1].pose.position.y)<0)
  angle = angle + M_PI;
 q = tf::createQuaternionMsgFromRollPitchYaw(0, 0, angle);
 quaternions.push_back(q);
}
move_base_msgs::MoveBaseGoal goal;
MoveBaseClient ac("move_base", true);
ros::NodeHandle nh1;
//Listener listener;

ros::Subscriber sub = nh1.subscribe("chatter", 1000, &Listener::callback, &listener);

ros::Rate loop_rate(10);
while (ros::ok())
{

  if((listener.count > 0.010)&&(i <(listener.plannnnn_.size()-1)))
 {
   ROS_INFO("dasd");
   goal.target_pose.header.frame_id = "map";
   goal.target_pose.header.stamp = ros::Time::now();
  //goal.target_pose.pose.position =MoveBase.plannnnn_[i];
  goal.target_pose.pose.position.x = listener.plannnnn_[i].pose.position.x;
   goal.target_pose.pose.position.y = listener.plannnnn_[i].pose.position.y;

   //quaternions = tf::createQuaternionMsgFromRollPitchYaw(0, 0, M_PI);
  goal.target_pose.pose.orientation = quaternions[i];
  // goal.target_pose.pose.orientation.w = 0.707;//listener.plannnnn_[i].pose.orientation.w;//1.0;//listener.planwwwww_[i].w;//1.0;
  // goal.target_pose.pose.orientation.x = 0.0;//listener.plannnnn_[i].pose.orientation.x;
  // goal.target_pose.pose.orientation.y = 0.0;//listener.plannnnn_[i].pose.orientation.y;
  // goal.target_pose.pose.orientation.z = 0.707;//listener.plannnnn_[i].pose.orientation.z;
   //goal.target_pose.pose.orientation.y = MoveBase.plannnnn_[
    ROS_INFO("Sending goal x:%lf  y:%lf    w:%lf",goal.target_pose.pose.position.x,goal.target_pose.pose.position.y, goal.target_pose.pose.orientation.w );
   ac.sendGoal(goal);
   ac.waitForResult();
   if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
     ROS_INFO("Hooray, the base moved 1 meter forward");
   else
     ROS_INFO("The base failed to move forward 1 meter for some reason");
   i++;
 }
  if(i== (listener.plannnnn_.size()-1))
  {
    listener.flag = 0;
     i=1;
     a=0;
  }

//}
  ros::spinOnce();
  loop_rate.sleep();
}

}

