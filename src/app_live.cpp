#include "app_live.hpp"
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace app;

LiveProcess::LiveProcess()
{
}

LiveProcess::LiveProcess(
	const std::string& _capture_filename,
	const std::string& save_folder,
	const std::string& _printer_name
)	: capture_filename(_capture_filename)
	, save_paths(save_folder)
	, config(_printer_name)
{
}

void LiveProcess::capture_and_process()
{
	std::cout << "shutter button pressed! capturing" << std::endl;
	system("gphoto2 --capture-image-and-download");

	std::this_thread::sleep_for(std::chrono::milliseconds(666));

	process_capture_and_save();

	print_processed_img();
}

void LiveProcess::try_process_new_capture()
{
	auto path = std::filesystem::path(capture_filename);
	auto new_last_write_time = std::filesystem::file_time_type();
	if (!std::filesystem::exists(path)) {
		return;
	}
	try {
		new_last_write_time = std::filesystem::last_write_time(path);
		if (new_last_write_time == last_write_time) {
			return;
		}
	} catch (const std::filesystem::filesystem_error& e) {
		return;
	}

	std::cout << "loading " << path
		<< ", taken at " << utils::to_time_t(last_write_time)
		<< " ..." << std::endl;

	config.load();
	// need to sleep because raspberry pi SD card is slow, and sometimes the
	// image is detected before it's been completely copied
	std::this_thread::sleep_for(std::chrono::milliseconds(666));

	process_capture_and_save();

	print_processed_img();

	std::filesystem::remove(capture_filename);
	last_write_time = new_last_write_time;
}

void LiveProcess::process_capture_and_save()
{
	auto img = imaging::load_img_and_process(
		capture_filename,
		averager.get_avg_img().size()
	);
	auto face = face::Face(img, detector);

	// reload if output folder doesn't exist
	if (!std::filesystem::exists(save_paths.folder)) {
		averager.load(save_paths);
	}
	averager.push(img.img, face);
	averager.save(save_paths);
	std::cout << "saved new avg" << std::endl;
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

void LiveProcess::print_processed_img()
{
	if (config.get_printer_name() != std::string("")) {
		auto print_cmd = std::string("lp -d ") + config.get_printer_name()
			+ std::string(" ")
			+ std::string(save_paths.img_proc.c_str());
		std::cout << "printing using this command: " << print_cmd << std::endl;
		system(print_cmd.c_str());
	} else {
		std::cout << "no printer name specified, can't print" << std::endl;
		return;
	}
}
