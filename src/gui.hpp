#ifndef __GUIS_HPP_
#define __GUIS_HPP_

#include "morph.hpp"
#include <opencv2/highgui.hpp>

class FaceAverager
{
	cv::Mat avg_img;
	cv::Rect avg_rect;
	std::vector<cv::Point2f> avg_face_vertices;
	int num_faces;
public:
	void push(const cv::Mat& img, const Face& face);
};

class FaceMorpher
{
	static std::string window_name;
	struct GuiParams {
		std::vector<cv::Mat> images;
		FaceDetector face_detector;
		cv::Mat faces[11];
		cv::Mat img1;
		cv::Mat img2;
		Face face1;
		Face face2;

		GuiParams(
			const std::vector<cv::Mat>& _images
		) : images(_images)
		{}
	};
	GuiParams params;

	static void select_face1_callback(int pos, void* ptr);
	static void select_face2_callback(int pos, void* ptr);
	static void trackbar_callback(int pos, void* ptr);
public:
	FaceMorpher(const std::vector<cv::Mat>& images);
};


#endif // __GUIS_HPP_
