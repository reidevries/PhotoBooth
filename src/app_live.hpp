#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include "face_averager.hpp"

namespace app
{

class Live {
	face::FaceAverager face_averager;
	face::FaceDetector face_detector;
public:
	void take_photo();
	void poll_button();
	void save_backup();
};


}

#endif // APP_LIVE_HPP_
