#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

ros::Publisher pub;

void cb(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);
    
    pub.publish(cv_ptr->toImageMsg()); 
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "mat_converter");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe ("/zed/depth/depth_registed", 1, cb);
    pub = n.advertise<sensor_msgs::Image> ("/agv/image/mat", 1);

    ros::spin();
    return 0;
}
