#include "app_live.hpp"

using namespace app;

void Live::check_for_new_capture(const std::string& filename)
{
	if (!utils::check_file_exists(filename)) {
		return;
	}

	std::cout << "loading " << filename << "..." << std::endl;
	auto img = NamedImg{
		filename,
		cv::imread(filename, cv::IMREAD_COLOR)
	};
	auto face = face::Face(img, detector);

	averager.push(img.img, face);

	save();
}

void Live::set_output_folder(std::string output_folder)
{
	if (output_folder.back() != '/') {
		output_folder += std::string("/");
	}
	output_img_filename = output_folder + std::string("avg.jpg");
	output_face_filename = output_folder + std::string("avg.face");
	num_faces_filename = output_folder + std::string("num_faces.u64");
}
