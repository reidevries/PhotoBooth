#include "app_live.hpp"
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace app;

void LiveProcess::button_pressed(uint32_t tick)
{
	//debounce
	if (tick - static_cast<u32>(last_tick_pressed) > 10000) {
		last_tick_pressed = tick;
	}
}

void LiveProcess::button_released(uint32_t tick)
{
	// check if the button has been held down
	auto time_since_pressed = tick - static_cast<u32>(last_tick_pressed);
	if (last_tick_pressed < 0) time_since_pressed = 0;

	// if the held for more than 30 seconds, restart computer
	if (time_since_pressed > 30000000) {
		if (time_since_pressed < 60000000) {
			std::cout << "button held for 30-60 seconds, rebooting" << std::endl;
			system("systemctl reboot");
		} else {
			std::cout << "possible error, button held for longer than a minute"
				<< std::endl;
		}
	// debounce by making sure the button was held for at least 10ms
	} else if (time_since_pressed > 10000) {
		auto time_since_released = tick - static_cast<u32>(last_tick_released);
		if (time_since_released > 30000000) { //30 seconds timer
			capturing = true;
			std::cout << "shutter button pressed and released!"
				<< " time elapsed since last release: " 
				<< last_tick_released-tick << std::endl;
			led_driver.countdown(1.5,0.5,3);
			capture_and_process();
			last_tick_released = tick;
		} else {
			std::cout << "didn't count button press because not enough time"
				<< " elapsed. last tick was " << last_tick_released
				<< " and current tick is " << tick << std::endl;
		}
	} else {
		std::cout << "button pressed for <10ms, debounced. Last release was "
			<< last_tick_released
			<< " current tick is " << tick << std::endl;
	}
}

LiveProcess::LiveProcess()
{
}

LiveProcess::LiveProcess(
	const std::string& _capture_filename,
	const std::string& save_folder
)	: capture_filename(_capture_filename)
	, save_paths(save_folder)
	, config()
{
}

void LiveProcess::capture_and_process()
{
	auto thread_ptr = led_driver.start_pulse(0.05, 0.002);

	system(
		"gphoto2 --capture-image-and-download \
		--force-overwrite --filename capt0000.jpg"
	);

	config.load();

	std::this_thread::sleep_for(std::chrono::milliseconds(666));

	auto path = std::filesystem::path(capture_filename);
	if (std::filesystem::exists(path)) {
		try {
			process_capture_and_save();
		} catch (const cv::Exception& e) {
			std::cout << "process_capture_and_save failed with this opencv"
				<< " error: " << e.msg << std::endl;
			std::cout << "printing most recent average instead" << std::endl;
		}
	} else {
		std::cout << "file " << capture_filename << " not found, printing"
			<< " most recent average" << std::endl;
	}

	print_processed_img();
	capturing = false;
	led_driver.stop_pulse(thread_ptr);
	last_tick_done_processing = gpioTick();
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

	capturing = true;
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
	capturing = false;
}

void LiveProcess::process_capture_and_save()
{
	auto img = imaging::load_img_and_process(
		capture_filename,
		averager.get_avg_img().size(),
		config.get_rotation()
	);
	auto face = face::Face(img, detector);

	// reload if output folder doesn't exist
	if (!std::filesystem::exists(save_paths.folder)) {
		averager.load(save_paths);
	}
	averager.push(img.img, face);
	averager.save(save_paths);
	std::cout << "saved new avg, generating photo strip" << std::endl;

	auto num_faces = averager.get_num_faces();
	io.generate_2x_photo_strip(averager.get_avg_img());
	
	std::cout << "saving photo strip" << std::endl;
	io.save_combined_photo_strip(save_paths.img_proc);

	save_to_exthd();
	
	++num_captures;
}

void LiveProcess::save_to_exthd()
{
	auto path = std::filesystem::path(config.get_ext_hd_path());
	if (!std::filesystem::exists(path)) {
		std::cout << "could not find folder at " << path << std::endl;
	}

	auto filename = std::stringstream();
	auto full_path = path;
	do {
		filename.str("");
		filename << std::setfill('0') << std::setw(8) << num_avgs_saved
			<< ".jpg";
		full_path = path / filename.str();
		++num_avgs_saved;
	} while (std::filesystem::exists(full_path));
	
	std::cout << "saving avg number " << num_avgs_saved
		<< " to " << full_path << std::endl;
	auto success = cv::imwrite(full_path, averager.get_avg_img()); 
	if (!success) {
		std::cout << "error saving average to " << full_path
			<< " for some unknown reason" << std::endl;
	}
}

void LiveProcess::set_save_paths(const std::string &folder)
{
	save_paths = OutputPaths(folder);
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

void LiveProcess::init_led_driver(int gpio)
{
	led_driver.init(gpio);
}
