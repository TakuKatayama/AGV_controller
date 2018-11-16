#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

ros::Publisher pub;

void cb(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    sensor_msgs::PointCloud2 floor = *msg;
    pub.publish(floor);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "floor_detection");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe ("/agv/pcl/output", 1, cb);
    pub = n.advertise<sensor_msgs::PointCloud2> ("/agv/pcl/floor", 1);

    ros::spin();
    return 0;
}
