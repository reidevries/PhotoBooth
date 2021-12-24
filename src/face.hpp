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

/**
 * FaceDetector basically gives RAII to two dlib classes for detecting faces
 * and facial landmarks.
 */
class FaceDetector
{
	dlib::frontal_face_detector detector;
	dlib::shape_predictor predictor;
public:
	/**
	 * Initializes the detector and predictor, deserializing the given .dat
	 * file into the predictor.
	 * @param predictor_filename specifies the .dat file name
	 */
	FaceDetector(const std::string& predictor_filename);

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

/**
 * Contains facial data generated from a photo of a Face
 */
class Face
{
	/// The rectangle bounding the face photo (in pixels)
	cv::Rect rect;
	/// The vertices of the facial landmarks detected by FaceDetector::predict
	std::vector<cv::Point2f> vertices;
	/// A list of triples, each has indices to three vertices forming a triangle
	std::vector<cv::Point3i> delaunay_indices;
	/// Used as part of Face constructor to store the img boundaries as vertices
	void store_boundary_points(const cv::Mat& img);
public:
	Face() {}
	/**
	 * Analyzes an image to generate facial data
	 * @param img The image containing the face
	 * @param face_detector An instance of `FaceDetector`
	 */
	Face(const cv::Mat& img, FaceDetector& face_detector);

	/**
	 * Gets a triangle from `vertices`
	 * @param indices A triple of indices to `vertices`
	 * @return A triangle in the format used by cv::getAffineTransform etc
	 */
	auto get_tri(const cv::Point3i& indices) const -> cv::Mat;

	auto get_rect() const -> cv::Rect { return rect; }
	auto get_vertices() const
		-> const std::vector<cv::Point2f>& { return vertices; }
	auto get_vertex_at(const int i) const
		-> cv::Point2f { return vertices.at(i); }
	auto get_delaunay_indices() const
		-> std::vector<cv::Point3i> { return delaunay_indices; }
};

#endif // __FACE_HPP_
