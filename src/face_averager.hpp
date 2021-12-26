#ifndef __FACE_AVERAGER_HPP_
#define __FACE_AVERAGER_HPP_

#include "morph.hpp"

namespace face
{

class FaceAverager
{
	cv::Mat avg_img;
	Face avg_face;
	int num_faces;
public:
	auto push(const cv::Mat& img, const Face& face) -> cv::Mat;
};

}

#endif // __FACE_AVERAGER_HPP_
