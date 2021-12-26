#ifndef __FACE_HPP_
#define __FACE_HPP_

#include "face_detector.hpp"

namespace face
{

/**
 * Contains facial data generated from a photo of a Face
 */
class Face
{
	/// The rectangle bounding the face in the photo (in pixels)
	cv::Rect rect;
	/// The rectangle of the size of the img itself (in pixels)
	cv::Rect img_rect;
	/// The vertices of the facial landmarks detected by FaceDetector::predict
	std::vector<cv::Point2f> vertices;
	/// A list of triples, each has indices to three vertices forming a triangle
	std::vector<cv::Point3i> delaunay_indices;
	/// stores whether the current list of delaunay indices is valid
	bool delaunay_valid = false;
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
	 * Allows setting a vertex, it will invalidate the delaunay indices
	 * @param vertex The new vertex to put into the vector
	 * @param i The index of the new vertex
	 */
	void set_vertex_at(const cv::Point2f& vertex, const int i);

	/**
	 * calculate the delaunay indices based on current vertices and rect
	 */
	void calc_delaunay();

	/**
	 * Gets a triangle from `vertices`
	 * @param indices A triple of indices to `vertices`
	 * @return A triangle in the format used by cv::getAffineTransform etc
	 */
	auto get_tri(const cv::Point3i& indices) const -> cv::Mat;

	/**
	 * gets the delaunay indices, throw an exception if it's invalid
	 */
	auto get_delaunay_indices() const -> std::vector<cv::Point3i>;

	void set_rect(const cv::Rect& rect) { this->rect = rect; }
	auto get_rect() const -> cv::Rect { return rect; }
	auto get_vertices() const
		-> const std::vector<cv::Point2f>& { return vertices; }
	auto get_vertex_at(const int i) const
		-> cv::Point2f { return vertices.at(i); }
};

}

#endif // __FACE_HPP_
