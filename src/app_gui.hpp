#ifndef __GUIS_HPP_
#define __GUIS_HPP_

#include "morph.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>
#include "face_averager.hpp"

namespace app
{
class FaceAverager
{
	static std::string window_name;
	struct GuiParams {
		std::vector<NamedImg> images;
		face::FaceDetector face_detector;
		std::vector<cv::Mat> avg_images;

		GuiParams(
			const std::vector<NamedImg>& _images
		) : images(_images)
		{}
	};
	GuiParams params;

	static void trackbar_callback(int pos, void* ptr);
public:
	FaceAverager(std::vector<NamedImg>& images);
};

class FaceMorpher
{
	static std::string window_name;
	struct GuiParams {
		std::vector<NamedImg> images;
		face::FaceDetector face_detector;
		cv::Mat faces[11];
		NamedImg img1;
		NamedImg img2;
		face::Face face1;
		face::Face face2;

		GuiParams(
			const std::vector<NamedImg>& _images
		) : images(_images)
		{}
	};
	GuiParams params;

	static void select_face1_callback(int pos, void* ptr);
	static void select_face2_callback(int pos, void* ptr);
	static void trackbar_callback(int pos, void* ptr);
public:
	FaceMorpher(std::vector<NamedImg>& images);
};

} // namespace gui

#endif // __GUIS_HPP_
