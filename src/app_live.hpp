#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include <filesystem>
#include <chrono>
#include "face_averager.hpp"
#include "config.hpp"
#include "shutter_button.hpp"

namespace app
{

class LiveProcess : public ShutterButton::Callback {
	/// the face averager, handles face averaging
	face::FaceAverager averager;
	/// the face detector, used as part of face averager
	face::FaceDetector detector;

	/// the filename of the capture from the camera
	std::string capture_filename;
	/// stores the output paths of each component required to save/load the
	/// state of `averager`
	OutputPaths save_paths;
	/// stores the last clock time a capture was found
	std::filesystem::file_time_type last_write_time;
	/// stores the name of the printer for lp in `print_processed_img`
	Config config;

protected:
	void button_pressed() override;

public:
	LiveProcess();
	LiveProcess(
		const std::string& capture_filename,
		const std::string& save_folder
	);

	/**
	 * Newer way of capturing an image and processing it, as a pigpio callback
	 */
	void capture_and_process();

	/**
	 * First, check if a new capture image exists. If it does, and it's more
	 * recent than the last capture image that was processed, then load it and
	 * push it to the average.
	 * Second, save the newly processed image, as well as the current average,
	 * in a folder that can be set with `set_save_paths`. 
	 * Third, print it using a system call to the linux utility `lp`.
	 * Finally, delete the processed capture.
	 */
	void try_process_new_capture();

	/**
	 * Process a new capture
	 */
	void process_capture_and_save();

	/**
	 * sets `save_paths` using the given folder.
	 */
	void set_save_paths(const std::string& folder);

	/**
	 * Loads the average stored in `save_paths`
	 */
	void load_avg();

	/**
	 * Prints the processed image saved at `save_paths`
	 */
	void print_processed_img();
};

}

#endif // APP_LIVE_HPP_
