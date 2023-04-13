#pragma once
#include <Kinect.h>



class kinect_api
{
private:
    IKinectSensor* sensor_;
    ICoordinateMapper* coordinate_mapper_;
    IMultiSourceFrameReader* multisource_frame_reader_;

    bool init_capture();
    void get_depth_frame(IMultiSourceFrame* multisource_frame);
    void get_color_frame(IMultiSourceFrame* multisource_frame);

public:
    bool initialized;
    kinect_api();
    ~kinect_api();
    bool frame_capture();

    int depth_height;
    int depth_width;
    UINT16* depth_data;
    
    int color_height;
    int color_width;
    BYTE* color_data;
    
    // CameraSpacePoint* depth2xyz;
    // ColorSpacePoint* depth2rgb; 
};
