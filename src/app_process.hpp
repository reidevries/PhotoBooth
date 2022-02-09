#ifndef APP_PROCESS_HPP_
#define APP_PROCESS_HPP_

#include "face_averager.hpp"

namespace app
{

void process_img(
	const std::string& img_filename,
	const std::string& output_folder
);

}

#endif // APP_PROCESS_HPP_
