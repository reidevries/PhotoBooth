#include "face_detector.hpp"

std::string FaceDetector::predictor_filename
	= "shape_predictor_68_face_landmarks.dat";

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
	auto vertices = predictor(img, rect);
	std::cout << "number of parts: " << vertices.num_parts() << std::endl;
	std::cout << "pixel pos of first part " << vertices.part(0) << std::endl;
	return convert::dlib_to_cv(vertices);
}
