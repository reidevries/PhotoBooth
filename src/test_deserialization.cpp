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
		std::cout << "pass" << std::endl;
	} else {
		for (u8 i = 0; i < std::max(serial.size(), serial2.size()); ++i) {
			if (i > serial.size()
				|| i > serial2.size()
				|| serial[i] != serial2[i]) {
				std::cout << serial[i] << "/" << serial2[i] << " "
					<< std::endl;
			}
		}
	}
}
