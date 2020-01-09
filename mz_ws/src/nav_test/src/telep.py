#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

def callback(data):
        twist = Twist()
        twist.linear.x = data.axes[1]
        twist.angular.z = data.axes[0]
        print('speed: %.2f, turn: %.2f'%(twist.linear.x,twist.angular.z))
        if data.buttons[2]==1:
             twist.linear.x = 0
             twist.angular.z = 0
             print("STOP!!")
        pub.publish(twist)

# Intializes everything
def start():
        # publishing to "cmd_vel" to control turtle1
        global pub
        #pub = rospy.Publisher('cmd_vel', Twist)
       # rate = rospy.Rate(10)
        # subscribed to joystick inputs on topic "joy"
        rospy.Subscriber("joy", Joy, callback)
        # starts the node
        rospy.init_node('Joy2Turtle')


        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            pub = rospy.Publisher('cmd_vel', Twist)
            rate.sleep()
        rospy.spin()

if __name__ == '__main__':
        start()
