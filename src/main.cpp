#include <iostream>
#include "kinect_api.h"
#include "Apriltag.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
    cout << "Inicio del programa!!" << endl;
    kinect_api sensor;

    VideoCapture cap(0);
    
    while (true)
    {
        if (true/*sensor.frame_capture()*/)
        {
            // cv::Mat frame = cv::Mat(1080, 1920, CV_8UC4, sensor.color_data);
            Mat frame;
            cap >> frame;
            
            Apriltag apriltag(tag36h11);
            apriltag.detect(frame);
            
            // Mostrar la imagen de entrada con los cuadros de detección
            cv::imshow("Apriltag detections", frame);
            if (cv::waitKey(1) == 27)
            {
                break;
            }
        }
    }
    cv::destroyAllWindows(); // Cierra todas las ventanas
    
    return 0;
}
