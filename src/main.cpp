#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include "kinect_api.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <apriltag/apriltag.h>
#include <apriltag/tag36h11.h>

using namespace cv;
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window);

// settings
constexpr unsigned int scr_width = 1000;
constexpr unsigned int scr_height = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices [512*424*3*2];
float vertices2[1080*1920*4];

int main()
{
    cout<<"Inicio del programa!!"<<endl;
    kinect_api sensor;
    apriltag_family_t* tagFamily = tag36h11_create();
    apriltag_detector_t* tagDetector = apriltag_detector_create();
    apriltag_detector_add_family(tagDetector, tagFamily);

    // Configurar los parámetros de detección
    tagDetector->quad_decimate = 1.0;
    tagDetector->quad_sigma = 0.0;
    tagDetector->nthreads = 1;
    tagDetector->debug = 0;
    tagDetector->refine_edges = 1;

    Size newSize(512, 424);

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    while (true)
    {
        if(true/*sensor.frame_capture()*/)
        {
            // sensor.copyDataToArray(vertices);
            // sensor.copyColorToArray(vertices2);
            // cv::Mat frame = cv::Mat(1080, 1920, CV_8UC4, sensor.color_data);
            Mat frame;
            cap >> frame;
            resize(frame, frame, newSize);
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
            // Mostrar la imagen de entrada con los cuadros de detección
            cv::imshow("Apriltag detections", frame);
            if (cv::waitKey(1) == 27) { // Espera 1ms y verifica si se ha presionado la tecla Esc
                break;
            }

            // Liberar la memoria
            for (size_t i = 0; i < detections_vec.size(); i++)
            {
                apriltag_detection_destroy(detections_vec[i]);
            }
        }
    }
    cv::destroyAllWindows(); // Cierra todas las ventanas
    apriltag_detector_destroy(tagDetector);
    tag36h11_destroy(tagFamily);
    
    
    // glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //
    // GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "KinectPointCloud", nullptr, nullptr);
    // if (window == nullptr)
    // {
    //     std::cout << "Failed to create GLFW window" << std::endl;
    //     glfwTerminate();
    //     return -1;
    // }
    // glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    //
    //
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //
    //
    // if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    // {
    //     std::cout << "Failed to initialize GLAD" << std::endl;
    //     return -1;
    // }
    //
    // glEnable(GL_DEPTH_TEST);
    //
    // Shader ourShader("src/shaders.vs", "src/shaders.fs");
    //
    //
    // unsigned int VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    //
    // ourShader.use();
    // glPointSize(3);
    //
    //
    //
    // while (!glfwWindowShouldClose(window))
    // {
    //     if(sensor.frameCapture())
    //     {
    //         sensor.copyDataToArray(vertices);
    //         sensor.copyColorToArray(vertices2);
    //         cv::Mat img = cv::Mat(1080, 1920, CV_32FC3, vertices2);
    //         imshow("image",img);
    //         
    //     }
    //     
    //     glBindVertexArray(VAO);
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //
    //     // position attribute
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    //     glEnableVertexAttribArray(0);
    //     // color attribute
    //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    //     glEnableVertexAttribArray(1);
    //    
    //     float currentFrame = static_cast<float>(glfwGetTime());
    //     deltaTime = currentFrame - lastFrame;
    //     lastFrame = currentFrame;
    //     
    //     processInput(window);
    //     
    //     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    //
    //     ourShader.use();
    //
    //
    //     glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)scr_width / (float)scr_height, 0.1f,
    //                                             100.0f);
    //     ourShader.setMat4("projection", projection);
    //
    //
    //     glm::mat4 view = camera.GetViewMatrix();
    //     ourShader.setMat4("view", view);
    //
    //
    //     glBindVertexArray(VAO);
    //     glm::mat4 model = glm::mat4(1.0f);
    //     ourShader.setMat4("model", model);
    //
    //     glDrawArrays(GL_POINTS, 0, sizeof(vertices) / sizeof(float) / 3);
    //     
    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    //
    // glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
