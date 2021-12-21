#include "face.hpp"

auto Face::get_delaunay() const -> cv::Subdiv2D
{
	auto subdiv = cv::Subdiv2D();
	subdiv.initDelaunay(rect);
	for (auto& point : shape) {
		subdiv.insert(point);
	}
}

auto Face::warp(
	const cv::Mat& img_src,
	const Face& face_src,
	const Face& face_dst
) -> cv::Mat
{
	return Face::warp(img_src, face_src, face_dst, 1);
}

auto Face::warp(
	const cv::Mat& img_src,
	const Face& face_src,
	const Face& face_dst,
	const float pos
) -> cv::Mat
{
	// get bounding boxes around the triangles
//	auto box_src = cv::boundingRect(face_src.shape);
//	auto box_dst = cv::boundingRect(face_dst.shape);

	auto mat_interp = utils::mean(mat_src, mat_dst, pos);
	auto affine = cv::getAffineTransform(mat_src, mat_interp);
	cv::Mat output;
	cv::warpAffine(img_src, output, affine, img_src.size());
	return output;
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
