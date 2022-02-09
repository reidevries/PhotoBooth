#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include "face_averager.hpp"

namespace app
{

class Live {
	face::FaceAverager averager;
	face::FaceDetector detector;
	u64 num_faces;

	std::string avg_img_filename;
	std::string avg_face_filename;
	std::string num_faces_filename;

public:
	void check_for_new_capture(const std::string& filename);

	void set_backup_filenames(
		const std::string& avg_img_filename,
		const std::string& avg_face_filename,
		const std::string& num_faces_filename
	);

	void save_backup();
	void load_backup();
};

}

#endif // APP_LIVE_HPP_
