#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	Face& face_src,
	Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img1 = img_src.clone();
	auto img2 = img_dst.clone();
	auto indices = face_src.get_delaunay_indices();
	for (auto& index : indices) {
		auto tri_src = face_src.get_tri(index);
		auto tri_dst = face_dst.get_tri(index);
		img1 = morph::warp_tri(
			img1,
			tri_src,
			tri_dst,
			pos
		);
		img2 = morph::warp_tri(
			img2,
			tri_dst,
			tri_src,
			1-pos
		);
	}
	return utils::mean(img1, img2, pos);
}

auto morph::warp_tri(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
) -> cv::Mat
{
	cv::Mat tri_interp = utils::mean(tri_src, tri_dst, pos);
	auto img_trans = affine_transform(img, tri_src, tri_interp);

	auto rect_dst = cv::boundingRect(tri_dst); // not sure if this will work
	cv::Mat mask = cv::Mat::zeros(
		img.size[0],
		img.size[1],
		img.type()
	);
	// needed for fillConvexPoly for some reason
	cv::Mat tri_interp_short;
	tri_interp.convertTo(tri_interp_short, CV_32S);
	cv::fillConvexPoly(
		mask,
		tri_interp_short,
		cv::Scalar(1.0, 1.0, 1.0, 1.0)
	);

	auto output = img.clone();
	img_trans.copyTo(output, mask);
	return output;
}

auto morph::affine_transform(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst
) -> cv::Mat
{
	// these have to be backwards from what the argument names would imply
	auto affine = cv::getAffineTransform(tri_src, tri_dst);
	cv::Mat output;
	cv::warpAffine(img, output, affine, img.size(), cv::BORDER_REFLECT_101);
	return output;
}
