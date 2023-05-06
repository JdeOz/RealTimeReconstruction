#include "Apriltag.h"


Apriltag::Apriltag(tag t)
{
    if(t == tag36h11)
    {
        tagFamily = tag36h11_create();
        tagDetector = apriltag_detector_create();
        apriltag_detector_add_family(tagDetector, tagFamily);
        tagDetector->quad_decimate = 1.0;
        tagDetector->quad_sigma = 0.0;
        tagDetector->nthreads = 1;
        tagDetector->debug = 0;
        tagDetector->refine_edges = 1;
    }
}

Apriltag::~Apriltag()
{
    apriltag_detector_destroy(tagDetector);
    tag36h11_destroy(tagFamily);
}

void Apriltag::detect(cv::Mat &frame)
{
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    image_u8_t apriltag_image = {
        .width = gray.cols, .height = gray.rows, .stride = gray.cols,
        .buf = gray.data
    };

    zarray_t* detections = apriltag_detector_detect(tagDetector, &apriltag_image);
    std::vector<apriltag_detection_t*> detections_vec;
    for (int i = 0; i < zarray_size(detections); i++)
    {
        apriltag_detection_t* det;
        zarray_get(detections, i, &det);
        detections_vec.push_back(det);
    }

    // Dibujar los cuadros de detección en la imagen de entrada
    for (size_t i = 0; i < detections_vec.size(); i++)
    {
        apriltag_detection_t* detection = detections_vec[i];
                
        cv::line(frame, cv::Point(detection->p[0][0], detection->p[0][1]),
                 cv::Point(detection->p[1][0], detection->p[1][1]), cv::Scalar(0, 255, 0), 2);
        cv::line(frame, cv::Point(detection->p[1][0], detection->p[1][1]),
                 cv::Point(detection->p[2][0], detection->p[2][1]), cv::Scalar(0, 255, 0), 2);
        cv::line(frame, cv::Point(detection->p[2][0], detection->p[2][1]),
                 cv::Point(detection->p[3][0], detection->p[3][1]), cv::Scalar(0, 255, 0), 2);
        cv::line(frame, cv::Point(detection->p[3][0], detection->p[3][1]),
                 cv::Point(detection->p[0][0], detection->p[0][1]), cv::Scalar(0, 255, 0), 2);
    }
}


