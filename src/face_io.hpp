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
	static auto load_num_faces(const std::string& filename) -> bool;
	static void save_num_faces(
		const std::string& filename,
		const u64 num_faces
	);

	auto generate_photo_strip(const cv::Mat& avg_img, const u64 num_faces); 
	auto save_combined_photo_strip(const std::string& filename);
};

}

#endif // __FACE_IO_HPP_
