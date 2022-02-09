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

	face::OutputPaths save_paths;
public:
	void check_for_new_capture(const std::string& filename);
	void set_save_paths(const std::string& folder);
};

}

#endif // APP_LIVE_HPP_
