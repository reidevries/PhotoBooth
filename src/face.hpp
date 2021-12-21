#ifndef __FACE_HPP_
#define __FACE_HPP_

#include "numbers.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include "utils.hpp"

struct Face
{
	dlib::rectangle rect;
	dlib::full_object_detection shape;
};

class FaceDetector
{
	dlib::frontal_face_detector detector;
	dlib::shape_predictor predictor;
public:
	FaceDetector(const std::string& predictor_filename);
	auto get_landmarks(const cv::Mat& img) -> Face;
};

#endif // __FACE_HPP_
