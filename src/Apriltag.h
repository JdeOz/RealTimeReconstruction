#pragma once
#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>
#include <opencv2/opencv.hpp>

enum tag {tag36h11};


class Apriltag
{
private:
    apriltag_family_t* tagFamily;
    apriltag_detector_t* tagDetector;
public:
    Apriltag(tag t);
    ~Apriltag();
    void detect(cv::Mat &frame);
    
};
