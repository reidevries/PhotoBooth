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

class Face
{
	static dlib::frontal_face_detector detector;
	static dlib::shape_predictor predictor;

	auto detect(const dlib::array2d<dlib::rgb_pixel>& img) -> dlib::rectangle;
	auto predict(
		const dlib::array2d<dlib::rgb_pixel>& img,
		const dlib::rectangle& rect
	) -> std::vector<cv::Point2f>;

	cv::Rect rect;
	std::vector<cv::Point2f> shape;
	std::vector<cv::Mat> delaunay;
public:
	static void init_dlib(const std::string& predictor_filename);

	Face(const cv::Mat& img);
	auto get_rect() const -> const cv::Rect& { return rect; }
	auto get_shape() const -> const std::vector<cv::Point2f>& { return shape; }
	auto get_delaunay() const
		-> const std::vector<cv::Mat>& { return delaunay; }
};

#endif // __FACE_HPP_
