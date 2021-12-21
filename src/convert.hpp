#ifndef __CONVERT_HPP_
#define __CONVERT_HPP_

#include "numbers.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlib/opencv.h>
#include <dlib/image_transforms.h>
#include <opencv2/imgproc.hpp>

namespace convert
{
auto cv_to_dlib_rgb(const cv::Mat& img) -> dlib::array2d<dlib::rgb_pixel>;
auto dlib_to_cv(const dlib::full_object_detection& shape)
	-> std::vector<cv::Point2f>;
}

#endif // __CONVERT_HPP_
