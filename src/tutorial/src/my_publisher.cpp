#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("/camera/image_raw", 1);
   VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
  Mat edges;
  namedWindow("edges",1);

  ros::Rate loop_rate(60);
  while (nh.ok()) {
    // place to store image
    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
    
    // If the frame is empty, break immediately
    if (frame.empty())
      break;
    //cv::Mat image = cv::imread(frame, CV_LOAD_IMAGE_COLOR);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
    pub.publish(msg);

    // Display the resulting frame
    imshow( "Frame", frame );
 
    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
    ros::spinOnce();
    loop_rate.sleep();
  }
}

