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
	cv::Mat bg_model, fg_model; // for grabCut algorithm

	/**
	 * Used to extract the vertices along the boundary of an image.
	 * @param img The image file in dlib format
	 * @param vertices The vertices will be pushed to this vector
	 */
	static void store_boundary_vertices(
		const dlib::array2d<dlib::rgb_pixel>& img,
		std::vector<cv::Point2f>& vertices
	);

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

	/**
	 * Runs the grabcut algorithm to extract a mask of the foreground
	 * @param img The image to run the algorithm on in CV format
	 * @param rect The rectangle containing the foreground in CV format
	 * @param threshold When `threshold = 2`, only the definite foreground
	 *	is included, when `threshold = 1`, the probable foreground is included,
	 *	when `threshold = 0`, the entire input rect is included
	 * @param iter_count How many iterations of the algorithm to run
	 */
	auto get_fg_mask(
		const cv::Mat& img,
		const cv::Rect& rect,
		const int threshold,
		const int iter_count
	) -> cv::Mat;
};

}

#endif // __FACE_DETECTOR_HPP_
