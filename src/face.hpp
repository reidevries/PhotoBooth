#ifndef __FACE_HPP_
#define __FACE_HPP_

#include "numbers.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include "utils.hpp"
#include "convert.hpp"

struct Face
{
	cv::Rect rect;
	std::vector<cv::Point2f> shape;

	auto get_delaunay() const -> cv::Subdiv2D;
};

class FaceDetector
{
	dlib::frontal_face_detector detector;
	dlib::shape_predictor predictor;

	// not sure if i will need this
	// auto get_img_boundary(const cv::Mat& img) -> std::vector<cv::Point2f>;

public:
	FaceDetector(const std::string& predictor_filename);
	auto get_landmarks(const cv::Mat& img) -> Face;
};

#endif // __FACE_HPP_
