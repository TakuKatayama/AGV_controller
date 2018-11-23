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
    int width = floor_ptr->image.cols;
    int height = floor_ptr->image.rows;
    cv::Mat feature_point = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);

    int u = 0;
    int v = 0;
    
    int left_feature_index;
    int right_feature_index;

    std::vector<cv::Point> left_feature;
    std::vector<cv::Point> right_feature;

    while(v == height - 1)
    {
        while(feature_point.ptr<unsigned char>(v)[u] == 255 || u == width - 1)
        {
            u++;
        }
        if(u != width - 1)
        {
            left_feature_index = u;
            u = width - 1;

            while(feature_point.ptr<unsigned char>(v)[u] == 255 || u == 0)
            {
                u--;
            }
            right_feature_index = u;

        }else
        {
            left_feature_index = 0;
            right_feature_index = 0;
        }
        left_feature.push_back(cv::Point(left_feature_index, v));
        right_feature.push_back(cv::Point(right_feature_index, v));
        v++;
        u = 0;
    }

    for(int i = 0; i < left_feature.size(); i++)
    {
        cv::circle(feature_point, left_feature[i], 20, cv::Scalar(0,200,0), -1, CV_AA);
        cv::circle(feature_point, right_feature[i], 20, cv::Scalar(200,0,0), -1, CV_AA);
    }

    sensor_msgs::ImagePtr pub_ptr = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::BGR8, feature_point).toImageMsg();
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
