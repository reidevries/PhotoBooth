#include "face_averager.hpp"

using namespace face;

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

	avg_face.set_rect(utils::mean(avg_face.get_rect(), face.get_rect(), coef));
	for (u32 i = 0; i < avg_face.get_vertices().size(); ++i) {
		avg_face.set_vertex_at(
			utils::mean(avg_face.get_vertex_at(i), face.get_vertex_at(i), coef),
			i
		);
	}

	auto img_warped = morph::warp_face(
		img,
		face,
		avg_face,
		1.0
	);

	avg_img = utils::mean(avg_img, img_warped, coef);
	return avg_img;
}
