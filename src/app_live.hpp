#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include <filesystem>
#include "face_averager.hpp"

namespace app
{

class LiveProcess {
	face::FaceAverager averager;
	face::FaceDetector detector;

	face::OutputPaths save_paths;
	std::filesystem::file_time_type last_write_time;
public:
	LiveProcess();
	LiveProcess(const std::string& save_folder);
	void check_for_new_capture(const std::string& filename);
	void set_save_paths(const std::string& folder);
};

}

#endif // APP_LIVE_HPP_
