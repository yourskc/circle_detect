#include <iostream>
#include <string>
#include <errno.h>
#include <stdexcept>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // Open the default camera (usually the first connected USB camera)
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open camera." << endl;
        return -1;
    }

    cv::Mat frame, gray;
    while (true) {
        cap >> frame; // Capture a new frame
        if (frame.empty()) {
            cerr << "Error: Captured empty frame." << endl;
            break;
        }

        // Convert to grayscale for better circle detection
        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

        // Detect circles
        vector<cv::Vec3f> circles;
        HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 16, 100, 30, 10, 100);

        // Output the number of circles detected
        cout << "Detected circles: " << circles.size() << endl;
            
        for (size_t i = 0; i < circles.size(); i++) {
            cv::Vec3f c = circles[i];
            std::cout << "Circle " << i + 1 << ": (" << c[0] << ", " << c[1] << ") Radius: " << c[2] << std::endl;

            // Draw the circle and its center
            cv::Point center(cvRound(c[0]), cvRound(c[1]));
            int radius = cvRound(c[2]);
            cv::circle(frame, center, radius, cv::Scalar(0, 255, 0), 2);
            cv::circle(frame, center, 2, cv::Scalar(0, 0, 255), 3);
        }
        
        // Display the frame
        cv::imshow("Circle Detection", frame);

        // Exit when ESC key is pressed
        
        if (waitKey(30) == 27) {
            cout << "ESC key pressed. Exiting..." << std::endl;
            break;
        }
        
    }

    cap.release();
    // destroyAllWindows();
    return 0;
}

