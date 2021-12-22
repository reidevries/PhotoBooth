#ifndef __MORPH_HPP_
#define __MORPH_HPP_

#include "utils.hpp"
#include "face.hpp"

namespace morph
{
auto warp_face(
	const cv::Mat& img_src,
	Face& face_src,
	Face& face_dst,
	const float pos
) -> cv::Mat;

auto warp_tri(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
) -> cv::Mat;

auto affine_transform(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst
) -> cv::Mat;

}

#endif // __MORPH_HPP_
