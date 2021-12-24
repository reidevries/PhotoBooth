#ifndef __MORPH_HPP_
#define __MORPH_HPP_

#include "utils.hpp"
#include "face.hpp"

namespace morph
{
auto warp_face(
	const cv::Mat& img,
	Face& face_src, // this should be const, but can't because of lib :(
	Face& face_dst, // same here
	const float pos
) -> cv::Mat;

auto warp_face_fading(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	Face& face_src, // this should be const, but can't because of lib :(
	Face& face_dst, // same here
	const float pos
) -> cv::Mat;

void warp_tri(
	cv::Mat& img_out,
	const cv::Mat& img_src,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
);

auto affine_transform(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst
) -> cv::Mat;

}

#endif // __MORPH_HPP_
