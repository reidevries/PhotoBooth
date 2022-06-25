#ifndef __FACE_IO_HPP_
#define __FACE_IO_HPP_

#include "numbers.hpp"
#include "imaging.hpp"
#include "config.hpp"

namespace face
{

class IO
{
	cv::Mat photo_strip[2];
public:
	void generate_photo_strip(const cv::Mat& avg_img, const u64 num_faces); 
	void save_combined_photo_strip(const std::string& filename);
};

}

#endif // __FACE_IO_HPP_
