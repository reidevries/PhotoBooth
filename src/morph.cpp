#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	Face& face_src,
	Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img_out = img_src.clone();
	auto indices = face_src.get_delaunay_indices();
	for (auto& index : indices) {
		auto tri_src = face_src.get_tri(index);
		auto tri_dst = face_dst.get_tri(index);
		img_out = morph::warp_tri(
			img_out,
			img_dst,
			tri_src,
			tri_dst,
			pos
		);
	}
	return img_out;
}

auto morph::warp_tri(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
) -> cv::Mat
{
	cv::Mat tri_interp = tri_src*(1-pos) + tri_dst*pos;
	auto img_trans = affine_transform(img_dst, tri_src, tri_interp);

	auto rect_dst = cv::boundingRect(tri_dst); // not sure if this will work
	cv::Mat mask = cv::Mat::zeros(
		img_src.size[0],
		img_src.size[1],
		img_src.type()
	);
	// needed for fillConvexPoly for some reason
	cv::Mat tri_interp_short;
	tri_interp.convertTo(tri_interp_short, CV_32S);
	cv::fillConvexPoly(
		mask,
		tri_interp_short,
		cv::Scalar(1.0, 1.0, 1.0, 1.0)
	);

	auto output = img_src.clone();
	img_trans.copyTo(output, mask);
	return output;
}

auto morph::affine_transform(
	const cv::Mat& img_src,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst
) -> cv::Mat
{
	auto affine = cv::getAffineTransform(tri_src, tri_dst);
	cv::Mat output;
	cv::warpAffine(img_src, output, affine, img_src.size(), cv::BORDER_REFLECT_101);
	return output;
}
