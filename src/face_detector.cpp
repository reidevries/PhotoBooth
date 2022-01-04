#include "face_detector.hpp"

using namespace face;

std::string FaceDetector::predictor_filename
	= "shape_predictor_68_face_landmarks.dat";

void FaceDetector::store_boundary_vertices(
	const dlib::array2d<dlib::rgb_pixel>& img,
	std::vector<cv::Point2f>& vertices
)
{
	auto w = img.nc() - 1;
	auto h = img.nr() - 1;
	std::cout << "img size is " << w << "x" << h << std::endl;
	vertices.push_back(cv::Point2f(  1,  1));
	vertices.push_back(cv::Point2f(  w,  1));
	vertices.push_back(cv::Point2f(  1,  h));
	vertices.push_back(cv::Point2f(  w,  h));
	vertices.push_back(cv::Point2f(w/2,  1));
	vertices.push_back(cv::Point2f(  1,h/2));
	vertices.push_back(cv::Point2f(w/2,  h));
	vertices.push_back(cv::Point2f(w,  h/2));
}

FaceDetector::FaceDetector()
	: detector(dlib::get_frontal_face_detector())
{
	dlib::deserialize(FaceDetector::predictor_filename.c_str()) >> predictor;
}

auto FaceDetector::detect(const dlib::array2d<dlib::rgb_pixel>& img)
	-> dlib::rectangle
{
	auto detect_threshold = 1;
	auto rects = detector(img, 1);
	// if no faces are detected, reduce the threshold a few times until some
	// are found
	while (rects.size() == 0) {
		--detect_threshold;
		rects = detector(img, detect_threshold);

		// if no faces are detected by now, assume the face takes up the whole
		// image
		if (detect_threshold < -3) {
			rects.push_back(dlib::rectangle(
				0,
				0,
				img.width_step(),
				img.size()/img.width_step())
			);
			break;
		}
	}
	std::cout << "num faces detected: " << rects.size()
		<< std::endl
		<< "threshold: " << detect_threshold
		<< std::endl;
	return rects[0];
}

auto FaceDetector::predict(
	const dlib::array2d<dlib::rgb_pixel>& img,
	const dlib::rectangle& rect
) -> std::vector<cv::Point2f>
{
	auto vertices_dlib = predictor(img, rect);
	auto vertices_cv = convert::dlib_to_cv(vertices_dlib);

	store_boundary_vertices(img, vertices_cv);


	return vertices_cv;
}

auto FaceDetector::get_fg_mask(
	const cv::Mat& img,
	const cv::Rect& rect,
	const int threshold,
	const int iter_count
) -> cv::Mat
{
	cv::Mat mask;
	cv::grabCut(
		img,
		mask,
		rect,
		bg_model,
		fg_model,
		iter_count,
		cv::GC_INIT_WITH_RECT
	);
	auto output_mask = cv::Mat(mask.size(), CV_8UC1);
	mask.convertTo(output_mask, CV_8UC1);
	if (threshold == 0) { // basically just includes the whole face
		output_mask = mask;
	} else if (threshold == 1) { // GC_PR_FGD and GC_FGD included
		output_mask = mask & cv::GC_FGD;
	} else if (threshold == 2) { // only GC_FGD included
		for (u16 row_i = 0; row_i < mask.rows; ++row_i) {
			uchar* p = output_mask.ptr(row_i);
			for (u16 col_i = 0; col_i < mask.cols; ++col_i) {
				auto p_i = *p;
				*p = p_i & cv::GC_FGD & ! ((p_i & cv::GC_PR_BGD) >> 1 );
				++p;
			}
		}
	}
	return output_mask*255;
}
