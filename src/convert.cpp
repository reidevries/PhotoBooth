#include "convert.hpp"

auto convert::cv_to_dlib_rgb(const cv::Mat& img)
	-> dlib::array2d<dlib::rgb_pixel>
{
	dlib::array2d<dlib::rgb_pixel> img_dlib;
	dlib::assign_image(img_dlib, dlib::cv_image<dlib::bgr_pixel>(img));
	return img_dlib;
}

auto convert::dlib_to_cv(const dlib::full_object_detection& shape)
	-> std::vector<cv::Point2f>
{
	std::vector<cv::Point2f> output;
	// its really annoying that I have to do this instead of just being able
	// to access the "parts" vector inside full_object_detection :(
	for (u64 i = 0; i < shape.num_parts(); ++i) {
		auto p = shape.part(i);
		output.push_back(cv::Point2f(p.x(), p.y()));
	}
	return output;
}

auto convert::dlib_to_cv(const dlib::rectangle& rect) -> cv::Rect
{
	return cv::Rect(
		rect.left(),
		rect.top(),
		rect.right() - rect.left(),
		rect.bottom() - rect.top()
	);
}
