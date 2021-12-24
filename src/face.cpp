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
	shape.push_back(cv::Point2f(w,  h/2));
	shape.push_back(cv::Point2f(w/2,h/2));
}

Face::Face(const cv::Mat& img, FaceDetector& face_detector)
{
	auto img_dlib = convert::cv_to_dlib_rgb(img);
	auto rect_dlib = face_detector.detect(img_dlib);
	shape = face_detector.predict(img_dlib, rect_dlib);
	rect = convert::dlib_to_cv(rect_dlib);
	store_boundary_points(img);

	auto subdiv = cv::Subdiv2D();
	auto img_rect = cv::Rect(0, 0, img.size[1], img.size[0]);
	subdiv.initDelaunay(img_rect);
	for (auto& point : shape) {
		subdiv.insert(point);
	}
	std::vector<cv::Vec6f> tri_list;
	subdiv.getTriangleList(tri_list);

	// the triangles need to be in a different format for getAffineTransform
	for (const auto& tri: tri_list) {
		auto index = cv::Point3i(-1,-1,-1);
		auto p1 = cv::Point2f(tri[0], tri[1]);
		auto p2 = cv::Point2f(tri[2], tri[3]);
		auto p3 = cv::Point2f(tri[4], tri[5]);
		for (u32 i = 0; i < shape.size(); ++i) {
			if (shape[i] == p1) {
				index.x = i;
			} else if (shape[i] == p2) {
				index.y = i;
			} else if (shape[i] == p3) {
				index.z = i;
			}
		}
		if (index.x == -1) {
			std::cerr << "point " << p1 << " not found in face!" << std::endl;
		}
		if (index.y == -1) {
			std::cerr << "point " << p2 << " not found in face!" << std::endl;
		}
		if (index.z == -1) {
			std::cerr << "point " << p3 << " not found in face!" << std::endl;
		}
		delaunay_indices.push_back(index);
	}
}

auto Face::get_tri(const cv::Point3i& indices) -> cv::Mat
{
	auto tri_o = cv::Mat(3,2,CV_32F);
	auto p0 = shape[indices.x];
	auto p1 = shape[indices.y];
	auto p2 = shape[indices.z];
	tri_o.at<float>(0,0) = p0.x;
	tri_o.at<float>(0,1) = p0.y;
	tri_o.at<float>(1,0) = p1.x;
	tri_o.at<float>(1,1) = p1.y;
	tri_o.at<float>(2,0) = p2.x;
	tri_o.at<float>(2,1) = p2.y;
	return tri_o;
}
