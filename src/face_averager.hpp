#ifndef __FACE_AVERAGER_HPP_
#define __FACE_AVERAGER_HPP_

#include "morph.hpp"
#include "imaging.hpp"
#include "config.hpp"
#include "face_io.hpp"
#include "output_paths.hpp"

namespace face
{

class FaceAverager
{
	cv::Mat avg_img;
	Face avg_face;
	u64 num_faces = 0;
	float param = 0.0;
	
	auto load_num_faces(const std::string& filename) -> u64;
	void save_num_faces(
		const std::string& filename,
		const u64 num_faces
	);
public:
	void set_param(float param);
	auto get_avg_img() const -> cv::Mat { return avg_img; }
	auto get_avg_face() const -> Face { return avg_face; }
	auto get_num_faces() const -> u64 { return num_faces; }
	void set(
		const cv::Mat& avg_img,
		const Face& avg_face,
		u64 num_faces
	);
	auto push(const cv::Mat& img, const Face& face) -> cv::Mat;
	auto process(
		const cv::Mat& img,
		const Face& face,
		const cv::Mat& avg_img,
		const Face& avg_face,
		const u64 num_faces
	) -> std::pair<cv::Mat, Face>;

	void save(const OutputPaths& paths);
	void load(const OutputPaths& paths);
};

}

#endif // __FACE_AVERAGER_HPP_
