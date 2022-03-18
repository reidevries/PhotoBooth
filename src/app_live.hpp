#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include <filesystem>
#include <chrono>
#include "face_averager.hpp"

namespace app
{

class LiveProcess {
	face::FaceAverager averager;
	face::FaceDetector detector;

	face::OutputPaths save_paths;
	std::filesystem::file_time_type last_write_time;
	std::string print_cmd;
public:
	LiveProcess();
	LiveProcess(
		const std::string& save_folder,
		const std::string& printer_name
	);

	/**
	 * check for a new
	 */
	void try_process_new_capture(const std::string& filename);
	void set_save_paths(const std::string& folder);
	void load_avg();
};

}

#endif // APP_LIVE_HPP_
