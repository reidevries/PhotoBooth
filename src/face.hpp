#ifndef __FACE_HPP_
#define __FACE_HPP_

#include "named_img.hpp"
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
	/// stores the estimated direction that the Face is pointing toward
	/// (0,0) is centre, (1,0) is right, (-1,1) is bottom left etc
	cv::Point2f direction;
	/// stores the filename of the img it came from for debugging
	std::string name;

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

	void estimate_direction();
public:
	Face() {}
	/**
	 * Analyzes an image to generate facial data
	 * @param img The image containing the face
	 * @param filename The filename of the image, for debugging purposes
	 * @param face_detector An instance of `FaceDetector`
	 */
	Face(const NamedImg& img, FaceDetector& face_detector);

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

	/**
	 * draws markers from the face onto the given img
	 */
	void draw_markers(cv::Mat& img) const;

	void set_rect(const cv::Rect& rect) { this->rect = rect; }
	auto get_rect() const -> cv::Rect { return rect; }
	auto get_vertices() const
		-> const std::vector<cv::Point2f>& { return vertices; }
	auto get_vertex_at(const int i) const
		-> cv::Point2f { return vertices.at(i); }
};

}

#endif // __FACE_HPP_
