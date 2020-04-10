#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include "hirop_msgs/setVord.h"

class SafetySpeed
{
public:
    SafetySpeed(moveit::planning_interface::MoveGroupInterface& g, ros::NodeHandle n);
private:
    ros::NodeHandle nh;
    moveit::planning_interface::MoveGroupInterface& move_group;
    ros::ServiceClient hsc3SetVord_client;
    ros::Subscriber detetor_sub;
    void subCallback(const std_msgs::Bool::ConstPtr& msg);    
};

SafetySpeed::SafetySpeed(moveit::planning_interface::MoveGroupInterface& g, ros::NodeHandle n)
:move_group{g}
{
    nh = n;
    detetor_sub = nh.subscribe("pedestrian_detection", 10, &SafetySpeed::subCallback, this);
    hsc3SetVord_client = nh.serviceClient<hirop_msgs::setVord>("hsc3SetVord");
};

// void subCallback(const std_msgs::Bool::ConstPtr& msg)
// {
//     hirop_msgs::setVord srv;
//     if(msg->data == true)
//     {
//         srv.request.vord = 30;
//     }
//     else
//     {
//         srv.request.vord = 80;
//     }
//     if(hsc3SetVord_client.call(srv))
//     {
//         ROS_INFO_STREAM("set Robot speed result: " << srv.response.ret );
//     }
//     else
//     {
//         ROS_INFO_STREAM("check /hsc3SetVord service");
//     }
// }

void SafetySpeed::subCallback(const std_msgs::Bool::ConstPtr& msg)
{
    if(msg->data)
    {
        ROS_INFO("slow down");
        move_group.setMaxVelocityScalingFactor(0.01);
        move_group.setMaxAccelerationScalingFactor(0.01);
    }
    else
    {
        ROS_INFO("normal speed");
        move_group.setMaxVelocityScalingFactor(1);
        move_group.setMaxAccelerationScalingFactor(1);
    }
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "safety_speed_brideg");
    ros::NodeHandle nh;
    // hsc3SetVord_client = nh.serviceClient<hirop_msgs::setVord>("hsc3SetVord");
    // ros::Subscriber detetor_sub = nh.subscribe("pedestrian_detection", 10, subCallback);
    moveit::planning_interface::MoveGroupInterface group("arm");
    SafetySpeed safety(group, nh);
    ros::spin();
    return 0;
}

