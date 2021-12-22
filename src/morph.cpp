#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img_src,
	Face& face_src,
	Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img_dst = img_src.clone();

	for (u64 i = 0; i < face_src.get_delaunay().size(); i+=3) {
		auto tri_src = face_src.get_delaunay()[i];
		auto tri_dst = face_dst.get_nearest_tri(tri_src);
		img_dst = morph::warp_tri(
			img_dst,
			tri_src,
			tri_dst,
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
	cv::Mat tri_interp = tri_src*(1-pos) + tri_dst*pos;
	auto img_trans = affine_transform(img, tri_src, tri_interp);

	auto rect_dst = cv::boundingRect(tri_dst); // not sure if this will work
	cv::Mat mask = cv::Mat::zeros(img.size[0], img.size[1], img.type());
	// needed for fillConvexPoly for some reason
	cv::Mat tri_interp_short;
	tri_interp.convertTo(tri_interp_short, CV_32S);
	cv::fillConvexPoly(
		mask,
		tri_interp_short, //not sure if this will work either
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
