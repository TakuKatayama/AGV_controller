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
    cv_bridge::CvImagePtr floor_ptr;
    floor_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_8UC1);
    int width = cv_ptr->image.cols;
    int height = cv_ptr->image.rows;
    cv_bridge::CvImagePtr floor_ptr;
    cv::Mat feature_point(cv::Size(width, height), CV_8UC1);

    double y = 0;
    double z = 0;
    
    int v_center = height / 2;
    int u_center = width / 2;

    int u = 0;
    int v = height - 1;

    while(u == width)
    {
        while(feature_point.ptr<unsigned char>(u)[v] != 255)
        {
            u++;
        }
    }

    for(int u = 0; u < width; u++)
    {
        for(int v = height; v <= 0; v--)
        {
            if (feature_point.ptr<unsigned char>(v)[u] != 255)
            int minYIdx = feature_point.ptr<unsigned char>(v)[u]
        }
    }

    sensor_msgs::ImagePtr pub_ptr = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::TYPE_8UC1, feature_point).toImageMsg();
    pub.publish(pub_ptr); 
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "feature_point_detecter");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe ("/agv/image/floor", 1, cb);
    pub = n.advertise<sensor_msgs::Image> ("/agv/image/feature_point", 1);

    ros::spin();
    return 0;
}
