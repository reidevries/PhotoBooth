#include "face_io.hpp"

using namespace face;

void IO::generate_photo_strip(const cv::Mat& avg_img, const u64 num_faces)
{
	photo_strip[num_faces%2] = imaging::process_aly_style(avg_img);
}

void IO::generate_2x_photo_strip(const cv::Mat& avg_img)
{
	photo_strip[0] = imaging::process_aly_style(avg_img);
	photo_strip[1] = photo_strip[0];
}

void IO::save_combined_photo_strip(const std::string& filename)
{
	cv::Mat combined;
	cv::hconcat(photo_strip[0], photo_strip[1], combined);
	auto success = cv::imwrite(filename, combined);
	if (!success) {
		std::cout << "error saving combined photo strip to " << filename
			<< " for some unknown reason" << std::endl;
	}
}

