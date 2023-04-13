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
