#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img_src,
	const Face& face_src,
	const Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img_dst = img_src.clone();

	auto tri_list_src = std::vector<cv::Vec6f>();
	auto tri_list_dst = std::vector<cv::Vec6f>();
	face_src.get_delaunay().getTriangleList(tri_list_src);
	face_dst.get_delaunay().getTriangleList(tri_list_dst);

	for (u64 i = 0; i < tri_list_dst.size(); ++i) {
		img_dst = morph::affine_transform(
			img_dst,
			tri_list_src[i],
			tri_list_dst[i],
			pos
		);
	}
	return img_dst;
}

auto morph::affine_transform(
	const cv::Mat& img_src,
	const cv::Vec6f& tri_src,
	const cv::Vec6f& tri_dst
) -> cv::Mat
{
	return morph::affine_transform(img_src, tri_src, tri_dst, 1);
}

auto morph::affine_transform(
	const cv::Mat& img_src,
	const cv::Vec6f& tri_src,
	const cv::Vec6f& tri_dst,
	const float pos
) -> cv::Mat
{
	// get bounding boxes around the triangles
//	auto box_src = cv::boundingRect(face_src.shape);
//	auto box_dst = cv::boundingRect(face_dst.shape);
	auto tri_interp = utils::mean(tri_src, tri_dst, pos);
	auto affine = cv::getAffineTransform(tri_src, tri_interp);
	cv::Mat output;
	cv::warpAffine(img_src, output, affine, img_src.size());
	return output;
}
