#ifndef __UTILS_HPP_
#define __UTILS_HPP_

#include "numbers.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlib/opencv.h>
#include <dlib/image_transforms.h>
#include <opencv2/imgproc.hpp>

namespace utils
{
void read_img_list(const std::string& filename, std::vector<cv::Mat>& images);

auto reshape_img_to_row(const cv::Mat& img) -> cv::Mat;
auto reshape_images_to_rows(const std::vector<cv::Mat> &images) -> cv::Mat;
auto reshape_row_to_img(const cv::Mat& row, const u64 img_rows) -> cv::Mat;
auto reshape_row_to_raw_grayscale_images(const cv::Mat& row, const u64 img_rows)
	-> cv::Mat;

void normalize_default(cv::Mat& mat);
}

#endif // __UTILS_HPP_
