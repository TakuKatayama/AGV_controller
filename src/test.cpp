#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

ros::Publisher pub;

void cb(const sensor_msgs::ImageConstPtr& msg)
{
    cv::Mat mat = cv::Mat::eye(100, 100, CV_8U) * 255;

    sensor_msgs::ImagePtr pub_ptr = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::MONO8, mat).toImageMsg();
    pub.publish(pub_ptr); 
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "test");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe ("/agv/image/floor", 1, cb);
    pub = n.advertise<sensor_msgs::Image> ("/agv/image/test", 1);

    ros::spin();
    return 0;
}
