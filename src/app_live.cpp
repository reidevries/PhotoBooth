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

	save();

	std::filesystem::remove(path);
}

void Live::set_output_folder(const std::string& output_folder)
{
	auto output_path = std::filesystem::path(output_folder);
	output_img_path = output_path / "avg.jpg";
	output_face_path = output_path / "avg.face";
	num_faces_path = output_path / "num_faces.u64";
}

void Live::save()
{
	auto avg_img = averager.get_avg_img();
	auto avg_face = averager.get_avg_face();
	auto num_faces = averager.get_num_faces();

	utils::save_num_faces(num_faces_path, num_faces);
	avg_face.save(output_face_path);
	cv::imwrite(output_img_path, avg_img);
}

void Live::load(const std::string& folder)
{
	auto output_path = std::filesystem::path(folder);
	auto avg_img_path = output_path / "avg.jpg";
	auto avg_face_path = output_path / "avg.face";
	auto backup_num_faces_path = output_path / "num_faces.u64";

	auto num_faces = utils::load_num_faces(backup_num_faces_path);
	auto avg_face = face::Face::load(avg_face_path);
	avg_face.set_name(avg_face_path);
	auto avg_img = cv::imread(avg_img_path, cv::IMREAD_COLOR);

	averager.set(avg_img, avg_face, num_faces);
}
