#ifndef __GUIS_HPP_
#define __GUIS_HPP_

#include "morph.hpp"
#include <opencv2/highgui.hpp>

namespace gui
{

class FaceAverager
{
	static std::string window_name;
	struct GuiParams {
		std::vector<cv::Mat> images;
		face::FaceDetector face_detector;
		cv::Mat faces[11];
		cv::Mat img1;
		cv::Mat img2;
		face::Face face1;
		face::Face face2;

		GuiParams(
			const std::vector<cv::Mat>& _images
		) : images(_images)
		{}
	};
	GuiParams params;
public:
};

class FaceMorpher
{
	static std::string window_name;
	struct GuiParams {
		std::vector<cv::Mat> images;
		face::FaceDetector face_detector;
		cv::Mat faces[11];
		cv::Mat img1;
		cv::Mat img2;
		face::Face face1;
		face::Face face2;

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

} // namespace gui

#endif // __GUIS_HPP_
