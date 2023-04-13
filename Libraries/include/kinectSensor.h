#pragma once
#include <Kinect.h>
#include <iostream>


template <typename T>
void SafeRelease(T& ptr)
{
    if (ptr)
    {
        ptr->Release();
        ptr = nullptr;
    }
}

inline bool errorAlert(const HRESULT hr, const std::string& msg)
{
    if (FAILED(hr))
    {
        std::cout << msg << std::endl;
        return true;
    }
    return false;
}

typedef struct rgb
{
    BYTE red;
    BYTE green;
    BYTE blue;
    BYTE alpha;
} rgb;

class kinectSensor
{
private:
    IKinectSensor* sensor;
    ICoordinateMapper* coordinateMapper;
    IMultiSourceFrameReader* multiSourceFrameReader;
    bool initCapture();
    void getDepthFrame(IMultiSourceFrame* multiSourceFrame);
    void getColorFrame(IMultiSourceFrame* multiSourceFrame);

public:
    bool initialized;
    kinectSensor();
    ~kinectSensor();
    bool frameCapture();
    void copyDataToArray(float *outputData);
    void copyColorToArray(float* outputData);

    int depthFrameHeight;
    int depthFrameWidth;
    int depthSize;
    UINT16* depthFrameData;
    CameraSpacePoint* depth2xyz;
    ColorSpacePoint* depth2rgb; 

    int colorFrameHeight;
    int colorFrameWidth;
    int colorSize;
    rgb* colorFrameData;
};
