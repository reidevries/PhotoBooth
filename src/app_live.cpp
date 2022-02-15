#include "app_live.hpp"
#include <stdlib.h>

using namespace app;

LiveProcess::LiveProcess()
{
}

LiveProcess::LiveProcess(
	const std::string& save_folder,
	const std::string& printer_name
) : save_paths(save_folder)
{
	if (printer_name != std::string("")) {
		print_cmd = std::string("lp -d ") + printer_name 
			+ std::string(" ")
			+ std::string(save_paths.img.c_str());
	} else {
		print_cmd = "";
	}
	std::cout << "using print command " << print_cmd << std::endl;
}

void LiveProcess::check_for_new_capture(const std::string& filename)
{
	auto path = std::filesystem::path(filename);
	if (!std::filesystem::exists(path)) {
		return;
	}
	try {
		auto new_last_write_time = std::filesystem::last_write_time(path);
		if (new_last_write_time == last_write_time) {
			return;
		}
		last_write_time = new_last_write_time;
	} catch (const std::filesystem::filesystem_error& e) {
		return;
	}

	std::cout << "loading " << path
		<< ", taken at " << utils::to_time_t(last_write_time)
		<< " ..." << std::endl;
	auto img = utils::load_img_and_process(filename);
	auto face = face::Face(img, detector);

	averager.push(img.img, face);
	averager.save(save_paths);
	std::cout << "saved new avg" << std::endl;

	system(print_cmd.c_str());

	std::filesystem::remove(filename);
}

void LiveProcess::set_save_paths(const std::string &folder)
{
	save_paths = face::OutputPaths(folder);
}

void LiveProcess::load_avg()
{
	if (utils::check_file_exists(save_paths.num_faces)) {
		averager.load(save_paths);
	}
}
