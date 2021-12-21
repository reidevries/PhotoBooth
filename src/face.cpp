#include "face.hpp"

auto Face::get_shape_mat() const -> cv::Mat
{
	auto output = cv::Mat(shape.size(), 1, CV_32F);
	for (u64 i = 0; i < shape.size(); ++i) {
		output.at<float>(i, 0) = shape[i].x;
		output.at<float>(i, 1) = shape[i].y;
	}
	return output;
}

auto Face::get_delaunay() const -> cv::Subdiv2D
{
	return cv::Subdiv2D();
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

	auto mat_src = face_src.get_shape_mat();
	auto mat_dst = face_dst.get_shape_mat();
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
