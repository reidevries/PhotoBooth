#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img_src,
	Face& face_src,
	Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img_dst = img_src.clone();

	for (u64 i = 0; i < 1; ++i) {
		auto tri_src = face_src.get_delaunay()[i];
		img_dst = morph::affine_transform(
			img_dst,
			tri_src,
			face_dst.get_nearest_tri(tri_src),
			pos
		);
	}
	return img_dst;
}

auto morph::affine_transform(
	const cv::Mat& img_src,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst
) -> cv::Mat
{
	return morph::affine_transform(img_src, tri_src, tri_dst, 1);
}

auto morph::affine_transform(
	const cv::Mat& img_src,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
) -> cv::Mat
{
	// get bounding boxes around the triangles
//	auto box_src = cv::boundingRect(face_src.shape);
//	auto box_dst = cv::boundingRect(face_dst.shape);
	auto affine = cv::getAffineTransform(tri_src, tri_dst);
	cv::Mat output;
	cv::warpAffine(img_src, output, affine, img_src.size());
	return output;
}
