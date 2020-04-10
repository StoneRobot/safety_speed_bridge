#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include "hirop_msgs/setVord.h"

ros::ServiceClient hsc3SetVord_client;
void subCallback(const std_msgs::Bool::ConstPtr& msg);

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "safety_speed_brideg");
    ros::NodeHandle nh;
    hsc3SetVord_client = nh.serviceClient<hirop_msgs::setVord>("hsc3SetVord");
    ros::Subscriber detetor_sub = nh.subscribe("photo", 10, subCallback);
    ros::spin();
    return 0;
}

void subCallback(const std_msgs::Bool::ConstPtr& msg)
{
    hirop_msgs::setVord srv;
    if(msg->data == true)
    {
        srv.request.vord = 30;
    }
    else
    {
        srv.request.vord = 80;
    }
    if(hsc3SetVord_client.call(srv))
    {
        ROS_INFO_STREAM("set Robot speed result: " << srv.response.ret );
    }
    else
    {
        ROS_INFO_STREAM("check /hsc3SetVord service");
    }
}