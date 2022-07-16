#include "face_averager.hpp"

using namespace face;

auto FaceAverager::load_num_faces(const std::string& filename) -> u64 {
	auto file = std::ifstream(filename, std::ifstream::in);
	if (!file) {
		std::cout << "file not found '" << filename
			<< "', assuming 0 faces" << std::endl;
		return 0;
	}
	u64 out = 0;
	file >> out;
	return out;
}

void FaceAverager::save_num_faces(
	const std::string& filename,
	const u64 num_faces
)
{
	auto file = std::ofstream(filename, std::ios::binary);
	if (!file) {
		std::cout << "could not open '" << filename
			<< "' for writing" << std::endl;
	}
	file << num_faces;
	file.close();
}

void FaceAverager::set_param(float param)
{
	if (param >= 0) {
		this->param = param;
	}
}

void FaceAverager::set(
	const cv::Mat& avg_img,
	const Face& avg_face,
	u64 num_faces
)
{
	this->avg_img = avg_img;
	this->avg_face = avg_face;
	this->num_faces = num_faces;
}

auto FaceAverager::push(const cv::Mat& img, const Face& face) -> cv::Mat
{
	auto coef = 1.0;
	if (num_faces > 0) {
		coef = fmax(
			fmin((1.0+param)/(num_faces+1+static_cast<int>(param)), 1.0),
			0.03
		);
	} else {
		// if num_faces == 0 then we need to initialize the averages
		std::cout << "as 0 faces have been processed,"
			<< " initializing FaceAverager" << std::endl;
		avg_face = face;
		avg_face.set_name("avg");
		avg_img = img;
		++num_faces;
		return avg_img;
	}

	auto old_avg_face = avg_face;
	old_avg_face.set_name("old avg face");

	// set the rect of avg_face to a new mean
	avg_face.set_rect(utils::mean(avg_face.get_rect(), face.get_rect(), coef));
	// set the vertices of the average to a new mean
	for (u32 i = 0; i < avg_face.get_vertices().size(); ++i) {
		avg_face.set_vertex_at(
			utils::mean(
				avg_face.get_vertex_at(i),
				face.get_vertex_at(i),
				coef
			),
			i
		);
	}

	old_avg_face.calc_delaunay();
	avg_img = morph::warp_face(
		avg_img,
		old_avg_face,
		avg_face,
		1.0
	);

	auto img_warped = morph::warp_face(
		img,
		face,
		avg_face,
		1.0
	);

	avg_img = utils::mean(avg_img, img_warped, coef);
	++num_faces;
	return avg_img;
}

auto FaceAverager::process(
	const cv::Mat& img,
	const Face& face,
	const cv::Mat& avg_img,
	const Face& avg_face,
	const u64 num_faces
) -> std::pair<cv::Mat, Face>
{
	set(avg_img, avg_face, num_faces);
	auto out = push(img, face);
	return std::pair<cv::Mat, Face>(out, avg_face);
}

void FaceAverager::save(const OutputPaths& paths)
{
	if (!std::filesystem::exists(paths.folder)) {
		std::cout << "directory " << paths.folder << " not found,"
			<< " creating it now" << std::endl;
		std::filesystem::create_directories(paths.folder);
	}
	save_num_faces(paths.num_faces, num_faces);
	avg_face.save(paths.face);
	cv::imwrite(paths.img, avg_img);
}

void FaceAverager::load(const OutputPaths& paths)
{
	if (!std::filesystem::exists(paths.folder)) {
		std::cout << "directory " << paths.folder << " not found,"
			<< " resetting" << std::endl;
		std::filesystem::create_directories(paths.folder);
		avg_img = cv::Mat();
		avg_face = face::Face();
		num_faces = 0;
	}
	avg_img = cv::imread(paths.img, cv::IMREAD_COLOR);
	avg_face = face::Face::load(paths.face);
	num_faces = load_num_faces(paths.num_faces);
	std::cout << "FaceAverager::load: read img from "
		<< paths.img << ", read face from"
		<< paths.face << ", read num_faces from"
		<< paths.num_faces << ". got this num faces: "
		<< num_faces << std::endl;
}
