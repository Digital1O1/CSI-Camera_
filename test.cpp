#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

void captureVideo(const std::string& pipeline, const std::string& windowName)
{
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open pipeline: " << pipeline << std::endl;
        return;
    }

    cv::Mat frame;

    while (true)
    {
        if (!cap.read(frame))
            break;

        cv::imshow(windowName, frame);

        // Exit the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q')
            break;
    }

    cap.release();
}

int main)
{
    // Define the gstreamer pipeline parameters for Camera 1 (sensor_id = 0)
    int capture_width = 1024;
    int capture_height = 768;
    int framerate = 30;
    int flip_method = 2;

    // Construct the gstreamer pipeline for Camera 1 (sensor_id = 0)
    std::string pipeline1 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=" + std::to_string(capture_width) + ", height=" + std::to_string(capture_height) + ", format=NV12, framerate=" + std::to_string(framerate) + "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // Construct the gstreamer pipeline for Camera 2 (sensor_id = 1)
    std::string pipeline2 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=" + std::to_string(capture_width) + ", height=" + std::to_string(capture_height) + ", format=NV12, framerate=" + std::to_string(framerate) + "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";

    // Start two threads to capture video from the two cameras
    std::string windowName1 = "Camera 1 (sensor-id=0)";
    std::string windowName2 = "Camera 2 (sensor-id=1)";
    std::thread t1(captureVideo, pipeline1, windowName1);
    std::thread t2(captureVideo, pipeline2, windowName2);

    // Wait for the threads to finish
    t1.join();
    t2.join();

    return 0;
}

