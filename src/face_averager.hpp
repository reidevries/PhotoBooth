#ifndef __FACE_AVERAGER_HPP_
#define __FACE_AVERAGER_HPP_

#include "morph.hpp"

class FaceAverager
{
	cv::Mat avg_img;
	cv::Rect avg_rect;
	std::vector<cv::Point2f> avg_vertices;
	int num_faces;
public:
	auto push(const cv::Mat& img, const Face& face) -> cv::Mat;
}

#endif // __FACE_AVERAGER_HPP_
