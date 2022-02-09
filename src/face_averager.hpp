#ifndef __FACE_AVERAGER_HPP_
#define __FACE_AVERAGER_HPP_

#include "morph.hpp"

namespace face
{

class FaceAverager
{
	cv::Mat avg_img;
	Face avg_face;
	u64 num_faces;
	float param = 1.0;
public:
	void set_param(float param);
	auto get_avg_img() -> cv::Mat { return avg_img; }
	auto get_avg_face() -> Face { return avg_face; }
	auto get_num_faces() -> u64 { return num_faces; }
	auto push(const cv::Mat& img, const Face& face) -> cv::Mat;
	auto process(
		const cv::Mat& img,
		const Face& face,
		const cv::Mat& avg_img,
		const Face& avg_face,
		const u64 num_faces
	) -> std::pair<cv::Mat, Face>;
};

}

#endif // __FACE_AVERAGER_HPP_
