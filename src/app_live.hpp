#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include <filesystem>
#include "face_averager.hpp"

namespace app
{

class Live {
	face::FaceAverager averager;
	face::FaceDetector detector;

	std::filesystem::path output_img_path;
	std::filesystem::path output_face_path;
	std::filesystem::path num_faces_path;

public:
	void check_for_new_capture(const std::string& filename);

	void set_output_folder(const std::string& output_folder);

	void save();
	void load(const std::string& folder);
};

}

#endif // APP_LIVE_HPP_
