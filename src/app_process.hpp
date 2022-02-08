#ifndef APP_PROCESS_HPP_
#define APP_PROCESS_HPP_

#include "morph.hpp"
#include "named_img.hpp"
#include "face_averager.hpp"

namespace app
{

void process_img(
	const std::string& img_filename,
	const std::string& avg_img_filename,
	const std::string& avg_face_filename,
	const std::string& num_faces_filename
);

}

#endif // APP_PROCESS_HPP_
