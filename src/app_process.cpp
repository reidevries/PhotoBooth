#include "app_process.hpp"

using namespace app;

void app::process_img(
	const std::string& img_filename,
	const std::string& avg_img_filename,
	const std::string& avg_face_filename,
	const std::string& num_faces_filename
)
{
	std::cout << "app::process_img using img filename " << img_filename
		<< " and avg img filename " << avg_img_filename << std::endl;
	face::FaceAverager averager;
	face::FaceDetector detector;

	auto num_faces = utils::load_num_faces(num_faces_filename);

	auto img = NamedImg{
		img_filename,
		cv::imread(img_filename, cv::IMREAD_COLOR)
	};
	auto face = face::Face(img, detector);

	auto avg_img = NamedImg{
		avg_img_filename,
		img.img
	};
	if (num_faces > 0) {
		auto avg_img = NamedImg{
			avg_img_filename,
			cv::imread(avg_img_filename, cv::IMREAD_COLOR)
		};
	}

	auto avg_face = face::Face::load(avg_face_filename);
	avg_face.set_name(avg_img_filename);
	if (avg_face.get_vertices().size() == 0) {
		std::cout << "app::process_img couldn't load face vertices,"
			<< " calculating them now... " << std::endl;
		avg_face = face::Face(avg_img, detector);
	}
	auto new_avg = averager.process(
		img.img,
		face,
		avg_img.img,
		avg_face,
		num_faces
	);
	utils::save_num_faces(num_faces_filename, ++num_faces);
	new_avg.second.save(avg_face_filename);
	cv::imwrite(avg_img_filename, new_avg.first);
}
