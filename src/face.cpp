#include "face.hpp"

auto Face::get_delaunay() const -> cv::Subdiv2D
{
	auto subdiv = cv::Subdiv2D();
	subdiv.initDelaunay(rect);
	for (auto& point : shape) {
		subdiv.insert(point);
	}
	return subdiv;
}

FaceDetector::FaceDetector(const std::string& predictor_filename)
	: detector(dlib::get_frontal_face_detector())
{
	dlib::deserialize(predictor_filename.c_str()) >> predictor;
}

auto FaceDetector::get_landmarks(const cv::Mat& img) -> Face
{
	auto img_dlib = convert::cv_to_dlib_rgb(img);
	auto rects = detector(img_dlib, 1);
	if (rects.size() == 0) {
		rects = detector(img_dlib, 0);
		if (rects.size() == 0) return Face{};
	}
	std::cout << "num faces detected: " << rects.size() << std::endl;
	auto target_rect = rects[0];

	auto shape = predictor(img_dlib, target_rect);
	std::cout << "number of parts: " << shape.num_parts() << std::endl;
	std::cout << "pixel pos of first part " << shape.part(0) << std::endl;
	std::cout << "pixel pos of second part " << shape.part(1) << std::endl;

	return Face{convert::dlib_to_cv(target_rect), convert::dlib_to_cv(shape)};
}
