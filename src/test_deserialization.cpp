#include "numbers.hpp"
#include "named_img.hpp"
#include "face.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	auto named_img = NamedImg{
		"test.jpg",
		cv::imread("test.jpg", cv::IMREAD_COLOR)
	};
	auto face_detector = face::FaceDetector();
	auto face = face::Face(named_img, face_detector);
	auto serial = face.serialize().str();
	auto face2 = face::Face(serial);
	auto serial2 = face2.serialize().str();
	if (serial == serial2) {
		std::cout << "pass!" << std::endl;
	} else {
		std::cout << "serialized forms do not match!" << std::endl;
	}
}
