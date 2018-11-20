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
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);
    int width = cv_ptr->image.cols;
    int height = cv_ptr->image.rows;
    //cv::Mat depth_map = cv_ptr->image;
    cv_bridge::CvImagePtr floor_ptr;
    cv::Mat floor(cv::Size(width, height), CV_8UC1);

    const double fku_l_vga = 349.824;
    double y = 0;
    double z = 0;
    
    int u_center = height / 2;

    for(int v = 0; v < height; v++)
    {
        for(int u = 0; u < width; u++)
        {
            z = cv_ptr->image.data[u + v * height];
            y = z * (u_center - v) / fku_l_vga;
            if (y < 0) {floor.at<unsigned char>(v, u) = 100;}
            else {floor.at<unsigned char>(v, u) = 0;}
        }
    }    

    cv::imshow("y", floor);
    cv::waitKey(3);
    sensor_msgs::ImagePtr pub_ptr = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::TYPE_8UC1, floor).toImageMsg();
    pub.publish(cv_ptr->toImageMsg()); 
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "floor_detecter");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe ("/zed/depth/depth_registered", 1, cb);
    pub = n.advertise<sensor_msgs::Image> ("/agv/image/mat", 1);

    ros::spin();
    return 0;
}
