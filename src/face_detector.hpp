#ifndef __FACE_DETECTOR_HPP_
#define __FACE_DETECTOR_HPP_

#include "numbers.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>

#include "utils.hpp"
#include "convert.hpp"

namespace face
{

/**
 * FaceDetector basically gives RAII to two dlib classes for detecting faces
 * and facial landmarks.
 */
class FaceDetector
{
	static std::string predictor_filename;
	dlib::frontal_face_detector detector;
	dlib::shape_predictor predictor;
public:
	/**
	 * Initializes the detector and predictor, deserializing the given .dat
	 * file with hardcoded predictor_filename as the predictor
	 */
	FaceDetector();

	/**
	 * Detects a face and returns the rectangle bounding the face.
	 * @param img The image file in dlib format that contains the face
	 * @return the rectangle surrounding the first detected face
	 */
	auto detect(const dlib::array2d<dlib::rgb_pixel>& img) -> dlib::rectangle;

	/**
	 * Predicts the facial landmarks of the given face image.
	 * @param img The image file in dlib format that contains the face
	 * @param rect The rectangle output of `detect` surrounding the face
	 * @return A vector of 2D points showing the predicted facial landmarks
	 */
	auto predict(
		const dlib::array2d<dlib::rgb_pixel>& img,
		const dlib::rectangle& rect
	) -> std::vector<cv::Point2f>;
};

}

#endif // __FACE_DETECTOR_HPP_