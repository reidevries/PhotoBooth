#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <gphoto2/gphoto2.h>
#include "face_averager.hpp"

namespace app
{

class Live {
	face::FaceAverager face_averager;
	face::FaceDetector face_detector;
	Camera* camera; //convert to unique_ptr
	GPContext* context; //convert to unique_ptr

	void create_context();

	static void context_error(
		GPContext* context,
		const char* str,
		void* data
	);

	static void context_status(
		GPContext* context,
		const char* str,
		void* data
	);

	static void error_dumper(
		GPLogLevel level,
		const char* domain,
		const char* str,
		void* data
	);
public:
	void init_cam();
	void take_photo();
	void poll_button();
	void save_backup();
};


}

#endif // APP_LIVE_HPP_
