#include "kinect_api.h"
#include "aux_lib.h"

kinect_api::kinect_api()
{
    sensor_ = nullptr;
    coordinate_mapper_ = nullptr;
    multisource_frame_reader_ = nullptr;

    depth_height = 0;
    depth_width = 0;
    depth_data = nullptr;

    color_height = 0;
    color_width = 0;
    color_data = nullptr;

    initialized = init_capture();
}

kinect_api::~kinect_api()
{
    SafeRelease(sensor_);
    SafeRelease(coordinate_mapper_);
}

bool kinect_api::init_capture()
{
    // Detectamos el kinect  y comprobamos que no hubo errores
    HRESULT hr = GetDefaultKinectSensor(&sensor_);
    if (errorAlert(hr, "Error al encontrar el sensor!")) return false;

    // Inicializamos el coordinateMaper
    sensor_->get_CoordinateMapper(&coordinate_mapper_);

    // Inicializamos el kinect(Lo abrimos) y comprobamos que no hubo errores
    hr = sensor_->Open();
    if (errorAlert(hr, "Error al abrir el sensor!")) return false;

    // Iniciamos el multiSource para leer multiples datos con el sensor y comprobamos que no hubo errores
    hr = sensor_->OpenMultiSourceFrameReader(FrameSourceTypes_Color |
                                             FrameSourceTypes_Depth,
                                             &multisource_frame_reader_);
    if (errorAlert(hr, "Error al abrir el multiReader!")) return false;

    // Si no hubo ningun error devolvemos true
    return true;
}


bool kinect_api::frame_capture()
{
    // Nos aseguramos que nodo se haya inicializado bien
    if (!initialized)
    {
        std::cout << "El sensor no se inico correctamente!" << std::endl;
        return false;
    }

    // Leemos los valores del sensor en el frame actual y los almacenamos en el multisource_frame
    IMultiSourceFrame* multisource_frame = nullptr;
    HRESULT hr = multisource_frame_reader_->AcquireLatestFrame(&multisource_frame);
    if (FAILED(hr)) return false;
    printf("copying data!\n"); //Mensaje debug TODO: quitar

    // Extraemos la profundidad y el color del multisource_frame
    get_depth_frame(multisource_frame);
    get_color_frame(multisource_frame);


    // depth2xyz = new CameraSpacePoint[static_cast<size_t>(depthSize)];
    // depth2rgb = new ColorSpacePoint[static_cast<size_t>(depthSize)];
    // coordinateMapper->MapDepthFrameToCameraSpace(depthSize, depth_data, depthSize, depth2xyz);
    // coordinateMapper->MapDepthFrameToColorSpace(depthSize, depth_data, depthSize, depth2rgb);

    SafeRelease(multisource_frame);
    return true;
}

void kinect_api::get_depth_frame(IMultiSourceFrame* multisource_frame)
{
    // Crearmos punteros a variables que vamos a usar
    IDepthFrameReference* depth_frame_reference = nullptr;
    IDepthFrame* depth_frame = nullptr;
    IFrameDescription* frame_description = nullptr;

    // Antes de optener los datos tenemos que pasar por la referencia
    HRESULT hr = multisource_frame->get_DepthFrameReference(&depth_frame_reference);
    if (errorAlert(hr, "Error al recibir la referencia del frame de profundidad!")) return;

    //Extraemos de la referencia la información de la profundidad
    hr = depth_frame_reference->AcquireFrame(&depth_frame);

    if (SUCCEEDED(hr))
    {
        // Si es la primera vez que capturamos la profundidad entonces extrames lo datos del tamaño
        if (depth_data == nullptr)
        {
            hr = depth_frame->get_FrameDescription(&frame_description);
            if (SUCCEEDED(hr))
            {
                frame_description->get_Height(&depth_height);
                frame_description->get_Width(&depth_width);
                depth_data = new UINT16[static_cast<size_t>(depth_width * depth_height)];
            }
        }
        // RESULTADO PRINCIPAL: exportamos la informacion de la profundidad en depth_data
        depth_frame->CopyFrameDataToArray(depth_width * depth_height, depth_data);
    }

    // Eliminamos todos los punteros 
    SafeRelease(depth_frame_reference);
    SafeRelease(depth_frame);
    SafeRelease(frame_description);
}

void kinect_api::get_color_frame(IMultiSourceFrame* multisource_frame)
{
    // Crearmos punteros a variables que vamos a usar
    IColorFrameReference* color_frame_reference = nullptr;
    IColorFrame* color_frame = nullptr;
    IFrameDescription* frame_description = nullptr;

    // Antes de optener los datos tenemos que pasar por la referencia
    HRESULT hr = multisource_frame->get_ColorFrameReference(&color_frame_reference);
    if (errorAlert(hr, "Error al recibir la referencia del frame de color!")) return;

    //Extraemos de la referencia la información del color
    hr = color_frame_reference->AcquireFrame(&color_frame);

    if (SUCCEEDED(hr))
    {
        // Si es la primera vez que capturamos imagen a color entonces extrames lo datos del tamaño
        if (color_data == nullptr)
        {
            hr = color_frame->get_FrameDescription(&frame_description);
            if (SUCCEEDED(hr))
            {
                frame_description->get_Height(&color_height);
                frame_description->get_Width(&color_width);
                color_data = new BYTE[static_cast<size_t>(color_height * color_width * 4)];
            }
        }
        // RESULTADO PRINCIPAL: exportamos la información de la profundidad en depth_data
        color_frame->CopyConvertedFrameDataToArray(color_height * color_width * 4, color_data, ColorImageFormat_Bgra);
    }

    SafeRelease(color_frame_reference);
    SafeRelease(color_frame);
    SafeRelease(frame_description);
}
