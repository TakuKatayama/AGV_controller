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
    
    int left_feature_index = 0;
    int right_feature_index = 0;
    int center_index = 0;

    std::vector<cv::Point2d> left_feature;
    std::vector<cv::Point2d> right_feature;
    std::vector<cv::Point2d> center_feature;

    bool flg1 = false;
    bool flg2 = false;

    // 両端の端からのindexを取得する処理
    for(int v = 0; v < height; v++)
    {
        // 左から |-->
        for(int u = 0; u < width; u++)
        {
            if(floor_ptr->image.ptr<unsigned char>(v)[u] == 200 && !flg1)
            {
                left_feature.push_back(cv::Point2d(u, v));
                flg1 = true;
                left_feature_index = u;
            }
        }

        // 右から <--|
        for(int u = width; u >= 0; u--)
        {
            if(floor_ptr->image.ptr<unsigned char>(v)[u] == 200 && !flg2)
            {
                right_feature.push_back(cv::Point2d(u, v));
                flg2 = true;
                right_feature_index = u;
            }
        }
        if(left_feature_index != 0 && right_feature_index != 0){
            center = ( right_feature_index + left_feature_index ) / 2;
            center_feature.push_back(cv::Point2d(center, v));
        }

        left_feature_index = 0;
        right_feature_index = 0;
        first_flg = false;
        second_flg = false;
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
