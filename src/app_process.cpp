#include "app_process.hpp"

using namespace app;

void app::process_img(
	const std::string& img_filename,
	const std::string& output_folder
)
{
	face::FaceAverager averager;
	face::FaceDetector detector;

	auto paths = face::OutputPaths(output_folder);
	std::cout << "app::process_img: using img filename " << img_filename
		<< " and avg img filename " << paths.img << std::endl;
	if (utils::check_file_exists(paths.num_faces)) {
		averager.load(paths);
	}

	auto img = NamedImg{
		img_filename,
		cv::imread(img_filename, cv::IMREAD_COLOR)
	};
	auto face = face::Face(img, detector);

	auto new_avg = averager.push(
		img.img,
		face
	);
	averager.save(paths);
}
