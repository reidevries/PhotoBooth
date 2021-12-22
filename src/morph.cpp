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
		img_dst = morph::warp_tri(
			img_dst,
			tri_src,
			face_dst.get_nearest_tri(tri_src),
			pos
		);
	}
	return img_dst;
}

auto morph::warp_tri(
	const cv::Mat& img,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
) -> cv::Mat
{
	auto tri_interp = tri_src*(1-pos) + tri_dst*pos;
	auto img_trans = affine_transform(img, tri_src, tri_interp);

	auto rect_dst = cv::boundingRect(tri_dst); // not sure if this will work
	cv::Mat mask = cv::Mat::zeros(img.size[0], img.size[1], img.type());
	cv::fillConvexPoly(
		mask,
		tri_dst, //not sure if this will work either
		cv::Scalar(1.0, 1.0, 1.0, 1.0)
	);

	auto output = img.clone();
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
