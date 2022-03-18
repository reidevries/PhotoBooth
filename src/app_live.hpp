#ifndef APP_LIVE_HPP_
#define APP_LIVE_HPP_

#include <fstream>
#include <filesystem>
#include <chrono>
#include "face_averager.hpp"

namespace app
{

class LiveProcess {
	/// the face averager, handles face averaging
	face::FaceAverager averager;
	/// the face detector, used as part of face averager
	face::FaceDetector detector;

	/// stores the output paths of each component required to save/load the
	/// state of `averager`
	face::OutputPaths save_paths;
	/// stores the last clock time a capture was found
	std::filesystem::file_time_type last_write_time;
	/// stores the name of the printer for lp in `print_processed_img`
	std::string printer_name;
public:
	LiveProcess();
	LiveProcess(
		const std::string& save_folder,
		const std::string& printer_name
	);

	/**
	 * First, check if a new capture image exists. If it does, and it's more
	 * recent than the last capture image that was processed, then load it and
	 * push it to the average.
	 * Second, save the newly processed image, as well as the current average,
	 * in a folder that can be set with `set_save_paths`. 
	 * Third, print it using a system call to the linux utility `lp`.
	 * Finally, delete the processed capture.
	 * @param filename The filename to look for the new capture at
	 */
	void try_process_new_capture(const std::string& filename);

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
