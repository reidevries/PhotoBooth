#include "face_averager.hpp"

auto FaceAverager::push(const cv::Mat& img, const Face& face) -> cv::Mat
{
	auto coef = 1.0;
	if (num_faces > 0) {
		coef = 1.0/num_faces;
	}
	if (fabsf(coef) <= 0.0000000001) {
		std::cout << "we may not be able to add more faces due to rounding :("
			<< std::endl;
		return avg_img;
	}

	avg_rect = utils::mean(avg_rect, face.get_rect(), coef);
	for (u32 i = 0; i < avg_vertices.size(); ++i) {
		auto& avg_vertex = avg_vertices[i];
		avg_vertex = utils::mean(avg_vertex, face.get_vertex_at(i), coef);
	}

	auto img_warped = img; //TODO

	avg_img = utils::mean(avg_img, img_warped, coef);
	return avg_img;
}
