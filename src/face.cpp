#include "face.hpp"

FaceDetector::FaceDetector(const std::string& predictor_filename)
	: detector(dlib::get_frontal_face_detector())
{
	dlib::deserialize(predictor_filename.c_str()) >> predictor;
}

auto FaceDetector::detect(const dlib::array2d<dlib::rgb_pixel>& img)
	-> dlib::rectangle
{
	auto rects = detector(img, 1);
	if (rects.size() == 0) {
		rects = detector(img, 0);
	}
	std::cout << "num faces detected: " << rects.size() << std::endl;
	return rects[0];
}

auto FaceDetector::predict(
	const dlib::array2d<dlib::rgb_pixel>& img,
	const dlib::rectangle& rect
) -> std::vector<cv::Point2f>
{
	auto shape = predictor(img, rect);
	std::cout << "number of parts: " << shape.num_parts() << std::endl;
	std::cout << "pixel pos of first part " << shape.part(0) << std::endl;
	return convert::dlib_to_cv(shape);
}

void Face::store_boundary_points(const cv::Mat& img)
{
	auto h = img.size[0];
	auto w = img.size[1];
	std::cout << "img has dims " << w << "x" << h << std::endl;
	shape.push_back(cv::Point2f(1,1));
	shape.push_back(cv::Point2f(w-1,1));
	shape.push_back(cv::Point2f(1,h-1));
	shape.push_back(cv::Point2f(w-1,h-1));
	shape.push_back(cv::Point2f((w-1)/2,1));
	shape.push_back(cv::Point2f(1,(h-1)/2));
	shape.push_back(cv::Point2f((w-1)/2,h-1));
	shape.push_back(cv::Point2f((w-1)/2,(h-1)/2));
}

Face::Face(const cv::Mat& img, FaceDetector& face_detector)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img);
	auto rect_dlib = face_detector.detect(img_dlib);
	shape = face_detector.predict(img_dlib, rect_dlib);
	store_boundary_points(img);
	rect = convert::dlib_to_cv(rect_dlib);

	auto subdiv = cv::Subdiv2D();
	subdiv.initDelaunay(rect);
	for (auto& point : shape) {
		subdiv.insert(point);
	}
	std::vector<cv::Vec6f> tri_list;
	subdiv.getTriangleList(tri_list);

	// the triangles need to be in a different format for getAffineTransform
	for (const auto& tri: tri_list) {
		auto tri_converted = cv::Mat(3, 2, CV_32F);
		tri_converted.at<float>(0,0) = tri[0];
		tri_converted.at<float>(0,1) = tri[1];
		tri_converted.at<float>(1,0) = tri[2];
		tri_converted.at<float>(1,1) = tri[3];
		tri_converted.at<float>(2,0) = tri[4];
		tri_converted.at<float>(2,1) = tri[5];
		delaunay.push_back(tri_converted);
	}
}
