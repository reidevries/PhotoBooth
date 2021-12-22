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
	auto w = img.size[1] - 1;
	auto h = img.size[0] - 1;
	std::cout << "img size is " << img.size << std::endl;
	shape.push_back(cv::Point2f(  1,  1));
	shape.push_back(cv::Point2f(  w,  1));
	shape.push_back(cv::Point2f(  1,  h));
	shape.push_back(cv::Point2f(  w,  h));
	shape.push_back(cv::Point2f(w/2,  1));
	shape.push_back(cv::Point2f(  1,h/2));
	shape.push_back(cv::Point2f(w/2,  h));
	shape.push_back(cv::Point2f(w/2,h/2));
	for (u64 i = 0; i < 8; ++i) {
		std::cout << shape[shape.size()-i-1] << std::endl;
	}
}

Face::Face(const cv::Mat& img, FaceDetector& face_detector)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img);
	auto rect_dlib = face_detector.detect(img_dlib);
	shape = face_detector.predict(img_dlib, rect_dlib);
	rect = convert::dlib_to_cv(rect_dlib);
	store_boundary_points(img);

	subdiv = cv::Subdiv2D();
	auto img_rect = cv::Rect(0, 0, img.size[1], img.size[0]);
	subdiv.initDelaunay(img_rect);
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

auto Face::get_nearest_tri(const cv::Mat& tri_i) -> cv::Mat&
{
	auto tri_o = cv::Mat(3,2,CV_32F);
	auto p = cv::Point2f();
	for (u8 i = 0; i < 3; ++i) {
		subdiv.findNearest(
			cv::Point2f(tri_i.at<float>(i,0), tri_i.at<float>(i,1)),
			&p
		);
		tri_o.at<float>(i,0) = p.x;
		tri_o.at<float>(i,1) = p.y;
	}
	return tri_o;
}
