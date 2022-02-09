#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include "face_averager.hpp"

namespace app
{

class Live {
	face::FaceAverager averager;
	face::FaceDetector detector;
	u64 num_faces;

	std::string output_img_filename;
	std::string output_face_filename;
	std::string num_faces_filename;

public:
	void check_for_new_capture(const std::string& filename);

	void set_output_folder(std::string output_folder);

	void save();
	void load();
};

}

#endif // APP_LIVE_HPP_
