#ifndef __UTILS_HPP_
#define __UTILS_HPP_

#include "numbers.hpp"
#include "named_img.hpp"
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
void read_img_list(
	const std::string& filename,
	std::vector<NamedImg>& images
);

auto reshape_img_to_row(const cv::Mat& img) -> cv::Mat;
auto reshape_images_to_rows(const std::vector<cv::Mat> &images) -> cv::Mat;
auto reshape_row_to_img(const cv::Mat& row, const u64 img_rows) -> cv::Mat;
auto reshape_row_to_raw_grayscale_images(const cv::Mat& row, const u64 img_rows)
	-> cv::Mat;

void normalize_default(cv::Mat& mat);

auto length_vec(const cv::Point2f& v) -> float;
auto normalize_vec(const cv::Point2f& v) -> cv::Point2f;
auto distance(const cv::Point2f& a, const cv::Point2f& b) -> float;


auto scale_point_to_other_rect(
	const cv::Point2f& p,
	const cv::Rect& src,
	const cv::Rect& dst
) -> cv::Point2f;

auto shift_scale_tri(
	cv::Mat& tri,
	const cv::Point2f& shift,
	const cv::Point2f& scale
);

auto scale_tri_to_other_rect(
	cv::Mat& tri,
	const cv::Rect& src,
	const cv::Rect& dst
);

auto mean(const cv::Rect& a, const cv::Rect& b, const float pos) -> cv::Rect;

template<typename T>
auto mean(const T& a, const T& b, const float pos) -> T
{
	return a*(1-pos) + b*pos;
}

auto load_num_faces() -> u64;
void save_num_faces(u64 num_faces);

}

#endif // __UTILS_HPP_
