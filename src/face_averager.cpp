#include "face_averager.hpp"

using namespace face;

auto FaceAverager::push(const cv::Mat& img, const Face& face) -> cv::Mat
{
	auto coef = 1.0;
	if (num_faces > 0) {
		coef = fmin(2.0/(num_faces+1), 1.0);
	} else {
		// if num_faces == 0 then we need to initialize the averages
		avg_face = face;
		avg_img = img;
		++num_faces;
		return avg_img;
	}

	if (fabsf(coef) <= 0.0000000001) {
		std::cout << "we may not be able to add more faces due to rounding :("
			<< std::endl;
		return avg_img;
	}

	auto old_avg_face = avg_face;

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
