#ifndef IMAGING_HPP_
#define IMAGING_HPP_

#include "named_img.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <locale>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace imaging
{

auto load_img_and_process(
	const std::string& filename,
	const cv::Size& expected_size
) -> NamedImg;

auto process_aly_style(const cv::Mat& img) -> cv::Mat;

}

#endif // IMAGING_HPP_
