#!/usr/bin/env python

""" move_base_square.py - Version 1.1 2013-12-20

    Command a robot to move in a square using move_base actions..

    Created for the Pi Robot Project: http://www.pirobot.org
    Copyright (c) 2012 Patrick Goebel.  All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.5
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details at:
    
    http://www.gnu.org/licenses/gpl.htmlPoint
      
"""

import rospy
import actionlib
from actionlib_msgs.msg import *
from geometry_msgs.msg import Pose, Point, Quaternion, Twist
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from tf.transformations import quaternion_from_euler
from std_msgs.msg import String
from math import radians, pi
global command_str 

def callback(data):
    global command_str 
    rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data) 
    command_str = str(data.data)
    rospy.loginfo(command_str)
    try:
        MoveBaseSquare()
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")
    
class MoveBaseSquare():
    def __init__(self):
        
        global command_str 
        rospy.on_shutdown(self.shutdown)
        
        locations = {'hall_foyer':Pose(Point(0.643, 0.2, 0.000), Quaternion(0.000, 0.000, 0.223, 0.975)) ,'hall_kitchen':Pose(Point(-1.994, 4.382, 0.000), Quaternion(0.000, 0.000, -0.670, 0.743)) ,'living_room_1':Pose(Point(0.720, 2.229, 0.000), Quaternion(0.000, 0.000, 0.786, 0.618))}
        '''
        locations['hall_foyer'] = Pose(Point(0.643, 4.72, 0.000), Quaternion(0.000, 0.000, 0.223, 0.975))
        locations['hall_kitchen'] = Pose(Point(-1.994, 4.382, 0.000), Quaternion(0.000, 0.000, -0.670, 0.743))
        locations['hall_bedroom'] = Pose(Point(-3.719, 4.401, 0.000), Quaternion(0.000, 0.000, 0.733, 0.680))
        locations['living_room_1'] = Pose(Point(0.720, 2.229, 0.000), Quaternion(0.000, 0.000, 0.786, 0.618))
        locations['living_room_2'] = Pose(Point(1.471, 1.007, 0.000), Quaternion(0.000, 0.000, 0.480, 0.877))
        locations['dining_room_1'] = Pose(Point(-0.861, -0.019, 0.000), Quaternion(0.000, 0.000, 0.892, -0.451))
        '''
        
       
            
        # Publisher to manually control the robot (e.g. to stop it, queue_size=5)
        self.cmd_vel_pub = rospy.Publisher('cmd_vel', Twist, queue_size=5)
        
        # Subscribe to the move_base action server
        self.move_base = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        
        
       
        rospy.loginfo("Waiting for move_base action server...")
        
        # Wait 60 seconds for the action server to become available
        self.move_base.wait_for_server(rospy.Duration(60))
        
        rospy.loginfo("Connected to move base server")
        rospy.loginfo("Starting navigation test")
        
        
        
        # Cycle through the four waypoints
        # while not rospy.is_shutdown():
          
            
            
            # Intialize the waypoint goal
        self.goal = MoveBaseGoal()
            
            # Use the map frame to define goal poses
        self.goal.target_pose.header.frame_id = 'map'
            
            # Set the time stamp to "now"
        self.goal.target_pose.header.stamp = rospy.Time.now()
            
            # Set the goal pose to the i-th waypoint
        self.goal.target_pose.pose = locations.get(command_str)
            
        rospy.loginfo(locations.get('hall_foyer'))
            
            # Start the robot moving toward the goal
        self.move(self.goal)
            
         
       
    def move(self, goal):
            # Send the goal pose to the MoveBaseAction server
            #self.move_base.send_goal(goal)
            self.move_base.send_goal(self.goal)
            # Allow 1 minute to get there
            finished_within_time = self.move_base.wait_for_result(rospy.Duration(60)) 
            
            # If we don't get there in time, abort the goal
            if not finished_within_time:
                self.move_base.cancel_goal()
                rospy.loginfo("Timed out achieving goal")
            else:
                # We made it!
                state = self.move_base.get_state()
                if state == GoalStatus.SUCCEEDED:
                    rospy.loginfo("Goal succeeded!")
                 
    
    def shutdown(self):
        rospy.loginfo("Stopping the robot...")
        # Cancel any active goals
        self.move_base.cancel_goal()
        rospy.sleep(2)
        # Stop the robot
        self.cmd_vel_pub.publish(Twist())
        rospy.sleep(1)

if __name__ == '__main__':
    try:
        # subcrible to the talker server receiving the command from the HIS
        rospy.init_node('nav_test', anonymous=False)
        rospy.Subscriber('chatter', String, callback)
        rospy.spin()
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")
