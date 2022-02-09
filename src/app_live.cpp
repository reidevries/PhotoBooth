#include "app_live.hpp"

using namespace app;

void Live::check_for_new_capture(const std::string& filename)
{
	auto path = std::filesystem::path(filename);
	if (std::filesystem::exists(path)) {
		return;
	}

	std::cout << "loading " << path << "..." << std::endl;
	auto img = NamedImg{
		filename,
		cv::imread(filename, cv::IMREAD_COLOR)
	};
	auto face = face::Face(img, detector);

	averager.push(img.img, face);

	averager.save(save_paths);

	std::filesystem::remove(path);
}

void Live::set_save_paths(const std::string &folder)
{
	save_paths = face::OutputPaths(folder);
}
