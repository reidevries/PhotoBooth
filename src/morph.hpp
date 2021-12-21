#ifndef __MORPH_HPP_
#define __MORPH_HPP_

#include "utils.hpp"
#include "face.hpp"

namespace morph
{
auto warp_face(
	const cv::Mat& img_src,
	const Face& face_src,
	const Face& face_dst,
	const float pos
) -> cv::Mat;

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
