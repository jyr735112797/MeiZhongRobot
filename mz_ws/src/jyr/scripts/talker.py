#!/usr/bin/env python3
# license removed for brevity
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32
from socket import *
import sys
import os
import json
false = False
null = ''
true = True

def talker():
    pub = rospy.Publisher('chatter', Float32, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    tcpClientSocket = socket(AF_INET, SOCK_STREAM)
    serAddr = ("192.168.1.101", 8899 )
    tcpClientSocket.connect(serAddr)
    top = 0
    bottom = 0
    left = 0
    right = 0
    rate = rospy.Rate(1) # 10hz
    flag = 0
    while not rospy.is_shutdown():
        hello_str = "hello world %s" % rospy.get_time()
        sendData = "face:0"
        flag = flag + 1
        if flag > 10:
            sendData = "start:0"
            flag = 0            
        if len(sendData)>0:
            tcpClientSocket.send(sendData.encode('utf-16'))
        else:
            break
        recvData = tcpClientSocket.recv(3072)
        print (recvData.decode('utf-16'))
        data = recvData.decode('utf-16')
        try:    
            a = json.loads(data)
            print(a['facedata_b1']['rect'])
            top = int(a['facedata_b1']['rect']['top'])
            bottom = int(a['facedata_b1']['rect']['bottom'])
            left = int(a['facedata_b1']['rect']['left'])
            right = int(a['facedata_b1']['rect']['right'])
            print(top, bottom, left, right)
        except ValueError:
            print("error")
        rect = (bottom-top)*(right-left)
        result = rect/(1920*1080)
        print(result)
        #rospy.loginfo(hello_str)
        pub.publish(result)
        rate.sleep()

if __name__ == '__main__':
    

    try:
        talker()
    except rospy.ROSInterruptException:
        pass
