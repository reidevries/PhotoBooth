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
	 * Used to extract a mask of the image foreground using GrabCut
	 */
	static auto get_fg_mask(
		const cv::Mat& img,
		const cv::Rect& rect,
		const int threshold = 1,
		const int iter_count = 3
	) -> cv::Mat;

	/**
	 * Used to get the edge points from the extracted foreground mask
	 */
	static auto get_fg_edge_points(
		const cv::Mat& mask,
		const cv::Rect& rect,
		const std::vector<cv::Point2f>& direction_vectors
	) -> std::vector<cv::Point2f>;

	/**
	 * Calls get_fg_edge_points with default direction vectors
	 */
	static auto get_fg_edge_points(
		const cv::Mat& img,
		const cv::Rect& rect,
		const cv::Point2f& facing_direction
	) -> std::vector<cv::Point2f>;

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
