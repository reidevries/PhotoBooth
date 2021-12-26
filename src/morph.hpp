#ifndef __MORPH_HPP_
#define __MORPH_HPP_

#include "utils.hpp"
#include "face.hpp"

class FaceAverager
{
	cv::Mat avg_img;
	cv::Rect avg_rect;
	std::vector<cv::Point2f> avg_face_vertices;
	int num_faces;
public:
	void push(const cv::Mat& img, const Face& face);
};

namespace morph
{
auto warp_face(
	const cv::Mat& img,
	const Face& face_src,
	const Face& face_dst,
	const float pos
) -> cv::Mat;

auto warp_face_fading(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	const Face& face_src,
	const Face& face_dst,
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
