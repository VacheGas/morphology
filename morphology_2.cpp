// User header

// Library
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

// Standard includes
#include <iostream>


struct userdata
{
    int erosion_elem = 0;
    int erosion_size = 0;
	int gradient_size = 0;
	int gradient_elem = 0;
    int dilation_elem = 0;
    int dilation_size = 0;
    std::string result_file;
    cv::Mat src;
};

void close(int, void* ud)
{
    userdata u = *static_cast<userdata*>(ud);
    int erosion_type = 0;
    if (u.erosion_elem == 0) {
        erosion_type = cv::MORPH_RECT;
    } else if (u.erosion_elem == 1) {
        erosion_type = cv::MORPH_CROSS;
    } else if (u.erosion_elem == 2) {
        erosion_type = cv::MORPH_ELLIPSE;
    }
    cv::Mat element = cv::getStructuringElement(erosion_type,
                     cv::Size(2 * u.erosion_size + 1, 2 * u.erosion_size + 1),
                     cv::Point(u.erosion_size, u.erosion_size ));
    cv::Mat dst;
	cv::dilate(u.src, dst, element);
    cv::erode(u.src, dst, element);
    cv::imshow("close Demo", dst);
}


void open(int, void* ud)
{
    userdata u = *static_cast<userdata*>(ud);
    int dilation_type = 0;
    if (u.dilation_elem == 0) {
        dilation_type = cv::MORPH_RECT;
    } else if (u.dilation_elem == 1) {
        dilation_type = cv::MORPH_CROSS;
    } else if (u.dilation_elem == 2) {
        dilation_type = cv::MORPH_ELLIPSE;
    }
    cv::Mat element = cv::getStructuringElement(dilation_type,
                         cv::Size(2 * u.dilation_size + 1,
                                  2 * u.dilation_size + 1),
                         cv::Point(u.dilation_size, u.dilation_size));
    cv::Mat dst;
	cv::erode(u.src, dst, element);
    cv::dilate(u.src, dst, element);

    cv::imshow("open Demo", dst);
}

void Grad(int, void* ud)
{
    userdata u = *static_cast<userdata*>(ud);
    int dilation_type = 0;
	dilation_type = cv::MORPH_GRADIENT;
    cv::Mat element = cv::getStructuringElement(dilation_type,
                         cv::Size(2 * u.gradient_size + 1,
                                  2 * u.gradient_size + 1),
                         cv::Point(-1, -1));
    cv::Mat dst;
    morphologyEx(u.src, dst,
                 cv::MORPH_GRADIENT, element);
    cv::imshow("open Demo", dst);
}

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv,
                                 "{@input | LinuxLogo.jpg | input image}");
    userdata u;
    u.src = cv::imread(parser.get<cv::String>( "@input" ), cv::IMREAD_COLOR);
    if (u.src.empty()) {
        return -1;
    }
    cv::namedWindow("close Demo", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("open Demo", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Gradient Demo", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Open Demo", u.src.cols,0);
    int const max_elem = 2;
    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",
                       "close Demo",
                    &u.erosion_elem, max_elem, close, &u);
    int const max_kernel_size = 21;
    cv::createTrackbar("Kernel size:\n 2n +1", "close Demo",
                    &u.erosion_size, max_kernel_size,
                    close, &u);
    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",
                       "Gradient Demo",
                    &u.gradient_elem, max_elem, Grad, &u);
cv::createTrackbar("Kernel size:\n 2n +1", "Gradient Demo",
                    &u.gradient_size, max_kernel_size,
                    Grad, &u);
    cv::createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse",
                       "open Demo",
                    &u.dilation_elem, max_elem,
                    open, &u);

    cv::createTrackbar("Kernel size:\n 2n +1", "open Demo",
                    &u.dilation_size, max_kernel_size,
                    open, &u);
    cv::waitKey(0);
    return 0;
}
