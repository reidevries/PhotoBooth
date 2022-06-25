#include "face_io.hpp"

using namespace face;

auto IO::load_num_faces(const std::string& filename) -> bool {
	auto file = std::ifstream(filename, std::ifstream::in);
	if (!file) {
		std::cout << "file not found '" << filename
			<< "', assuming 0 faces" << std::endl;
		return 0;
	}
	u64 out = 0;
	file >> out;
	return out;
}

auto IO::save_num_faces(const std::string& filename, const u64 num_faces)
{
	auto file = std::ofstream(filename, std::ios::binary);
	if (!file) {
		std::cout << "could not open '" << filename
			<< "' for writing" << std::endl;
	}
	file << num_faces;
	file.close();
}

auto IO::generate_photo_strip(const cv::Mat& avg_img, const u64 num_faces)
{
	photo_strip[num_faces%2] = imaging::process_aly_style(avg_img);
}

auto IO::save_combined_photo_strip(const std::string& filename)
{
	cv::Mat combined;
	cv::hconcat(photo_strip[0], photo_strip[1], combined);
	cv::imwrite(filename, combined);
}
