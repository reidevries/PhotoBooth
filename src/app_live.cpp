#include "app_live.hpp"

using namespace app;

LiveProcess::LiveProcess()
{
}

LiveProcess::LiveProcess(const std::string& save_folder)
	: save_paths(save_folder)
{
}

void LiveProcess::check_for_new_capture(const std::string& filename)
{
	auto path = std::filesystem::path(filename);
	if (!std::filesystem::exists(path)) {
		return;
	}
	auto new_last_write_time = std::filesystem::last_write_time(path);
	if (new_last_write_time == last_write_time) {
		return;
	}
	last_write_time = new_last_write_time;

	std::cout << "loading " << path << "..." << std::endl;
	auto img = NamedImg{
		filename,
		cv::imread(filename, cv::IMREAD_COLOR)
	};
	auto face = face::Face(img, detector);

	averager.push(img.img, face);

	averager.save(save_paths);
}

void LiveProcess::set_save_paths(const std::string &folder)
{
	save_paths = face::OutputPaths(folder);
}
