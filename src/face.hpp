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

class FaceDetector
// basically just gives RAII to frontal_face_detector and shape_predictor
{
	dlib::frontal_face_detector detector;
	dlib::shape_predictor predictor;
public:
	FaceDetector(const std::string& predictor_filename);
	auto detect(const dlib::array2d<dlib::rgb_pixel>& img) -> dlib::rectangle;
	auto predict(
		const dlib::array2d<dlib::rgb_pixel>& img,
		const dlib::rectangle& rect
	) -> std::vector<cv::Point2f>;
};

class Face
{
	cv::Rect rect;
	std::vector<cv::Point2f> shape;
	std::vector<cv::Mat> delaunay;

	void store_boundary_points(const cv::Mat& img);
public:
	Face() {}
	Face(const cv::Mat& img, FaceDetector& face_detector);
	auto get_delaunay() const
		-> const std::vector<cv::Mat>& { return delaunay; }
};

#endif // __FACE_HPP_
