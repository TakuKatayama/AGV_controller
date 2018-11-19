#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

ros::Publisher pub;

void cb(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);
    cv::Mat msgmat = cv_ptr->image;
    cv::Mat floor = cv::Mat::zeros(msgmat.rows, msgmat.cols, CV_8U);

    for(int v = 0; v < msgmat.cols; v++)
    {
        for(int u = 0; u < msgmat.rows; u++)
        {
            double y = msgmat.ptr<double>(u)[v] * 1;
            if (y < 0.5)
            {
                floor.ptr<uchar>(u)[v] = 128;
            }
        }
    }    

    sensor_msgs::ImagePtr pub_ptr = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::MONO8, floor).toImageMsg();

    pub.publish(pub_ptr); 
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
