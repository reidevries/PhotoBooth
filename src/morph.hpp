#ifndef __MORPH_HPP_
#define __MORPH_HPP_

#include "utils.hpp"

namespace morph
{
auto affine_transform(
	const cv::Mat& img,
	const cv::Vec6f& tri_src,
	const cv::Vec6f& tri_dst
) -> cv::Mat;

auto affine_transform(
	const cv::Mat& img,
	const cv::Vec6f& tri_src,
	const cv::Vec6f& tri_dst,
	const float pos
) -> cv::Mat;
}

#endif // __MORPH_HPP_
