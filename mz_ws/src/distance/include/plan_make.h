#ifndef PLAN_MAKE_H
#define PLAN_MAKE_H

#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <angles/angles.h>
//
#include <costmap_2d/costmap_2d.h>
#include <costmap_2d/costmap_2d_publisher.h>
#include <costmap_2d/cost_values.h>
#include <costmap_2d/footprint.h>
#include <base_local_planner/costmap_model.h>

#include <base_local_planner/footprint_helper.h>
#include <nav_core/base_global_planner.h>
#include <navfn/navfn_ros.h>
//#include <global_planner/planner_core.h>
#include <planner_core.h>
//
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//
#include <iostream>
#include <cstdio>
#include <set>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
//
//#include "line_function.h"
//#include "adjacency_graph.h"


struct MapParams
{
    std_msgs::Header header;
    float resolution;
    int width;
    int height;
    geometry_msgs::Pose origin;
    std::vector<uchar> data;
};
class CoveragePlanner{
public:
  CoveragePlanner();
  bool mapGridToCostmap(const MapParams &map, costmap_2d::Costmap2D* costmap);
  bool saveCostmap(std::string file_name, costmap_2d::Costmap2D* costmap);
  void buildInflationCostmap(base_local_planner::CostmapModel* costmap_model, double distance,
                                             costmap_2d::Costmap2D* inflation_costmap);
  bool makeGlobalPlan(const geometry_msgs::Point &start,
                                       const geometry_msgs::Point &goal,
                                       double interval,
                                      std::vector<geometry_msgs::Point> &plan);

  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;
 std::string map_frame_;
 MapParams &map_data;
double robot_radius_;
/*base_local_planner::FootprintHelper* footprint_helper_;
  costmap_2d::Costmap2D*  costmap_;
  base_local_planner::CostmapModel* costmap_model_;
  costmap_2d::Costmap2D*  inflation_costmap_;
  base_local_planner::CostmapModel*  inflation_costmap_model_;
*/
//
    std::vector<geometry_msgs::Point> footprint_spec_;
    base_local_planner::FootprintHelper* footprint_helper_;
    costmap_2d::Costmap2D* costmap_;
    base_local_planner::CostmapModel* costmap_model_;
    costmap_2d::Costmap2DPublisher* costmap_publisher_;
    //
    costmap_2d::Costmap2D* inflation_costmap_;
    base_local_planner::CostmapModel* inflation_costmap_model_;
//    navfn::NavfnROS* global_planner_;
    global_planner::GlobalPlanner* global_planner_;
    //
    costmap_2d::Costmap2DPublisher* region_costmap_publisher_;
    costmap_2d::Costmap2DPublisher* inflation_costmap_publisher_;
    };

#endif // PLAN_MAKE_H
