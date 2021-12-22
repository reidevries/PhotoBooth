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

	for (u64 i = 0; i < 1; ++i) {
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

auto convert_thing(const cv::Vec6f& input) -> cv::Mat
{
	auto output = cv::Mat(3, 2, CV_32F);
	output.at<float>(0,0) = input[0];
	output.at<float>(0,1) = input[1];
	output.at<float>(1,0) = input[2];
	output.at<float>(1,1) = input[3];
	output.at<float>(2,0) = input[4];
	output.at<float>(2,1) = input[5];
	return output;
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
	auto tri_src_mat = convert_thing(tri_src);
	auto tri_dst_mat = convert_thing(tri_dst);
	auto affine = cv::getAffineTransform(tri_src_mat, tri_dst_mat);
	cv::Mat output;
	cv::warpAffine(img_src, output, affine, img_src.size());
	return output;
}
