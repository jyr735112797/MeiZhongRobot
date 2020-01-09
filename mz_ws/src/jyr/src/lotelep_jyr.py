#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Joy

twist = Twist()

def callback(data):
	#global twist #= Twist()
        twist.linear.x = data.axes[1]*5
        twist.angular.z = data.axes[0]*5
        print('speed: %.2f, turn: %.2f'%(twist.linear.x,twist.angular.z))
        if data.buttons[2]==1:
             twist.linear.x = 0
             twist.angular.z = 0
             print("STOP!!")
	#pub.publish(twist)
 
# Intializes everything
def start():
	# publishing to "cmd_vel" to control turtle1
	global pub 
	global twist
	pub = rospy.Publisher('cmd_vel', Twist)
        rospy.init_node('Joy2Turtle')
        #rate = rospy.Rate(20)
	while not rospy.is_shutdown():
	# subscribed to joystick inputs on topic "joy"
		rospy.Subscriber("joy", Joy, callback)

                pub.publish(twist)
                #rate = rospy.Rate(60)
	# starts the node
        	#rate.sleep()
	
        rospy.spin()
 
if __name__ == '__main__':
	start()
