#include "morph.hpp"

auto morph::warp_face(
	const cv::Mat& img,
	const face::Face& face_src,
	const face::Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img_out = img.clone();
	auto indices = face_src.get_delaunay_indices();
	for (auto& index : indices) {
		auto tri_src = face_src.get_tri(index);
		auto tri_dst = face_dst.get_tri(index);
		morph::warp_tri(
			img_out,
			img,
			tri_src,
			tri_dst,
			pos
		);
	}
	return img_out;
}

auto morph::warp_face_fading(
	const cv::Mat& img_src,
	const cv::Mat& img_dst,
	const face::Face& face_src,
	const face::Face& face_dst,
	const float pos
) -> cv::Mat
{
	auto img1 = img_src.clone();
	auto img2 = img_dst.clone();
	auto indices = face_src.get_delaunay_indices();
	for (auto& index : indices) {
		auto tri_src = face_src.get_tri(index);
		auto tri_dst = face_dst.get_tri(index);
		morph::warp_tri(
			img1,
			img_src,
			tri_src,
			tri_dst,
			pos
		);
		morph::warp_tri(
			img2,
			img_dst,
			tri_dst,
			tri_src,
			1-pos
		);
	}
	return utils::mean(img1, img2, pos);
}

void morph::warp_tri(
	cv::Mat& img_out,
	const cv::Mat& img_src,
	const cv::Mat& tri_src,
	const cv::Mat& tri_dst,
	const float pos
)
{
	cv::Mat tri_interp = utils::mean(tri_src, tri_dst, pos);
	auto img_trans = affine_transform(img_src, tri_src, tri_interp);

	cv::Mat mask = cv::Mat::zeros(
		img_out.size[0],
		img_out.size[1],
		img_out.type()
	);
	// triConvexPoly wants a short datatype for some reason
	cv::Mat tri_interp_short;
	tri_interp.convertTo(tri_interp_short, CV_32S);
	cv::fillConvexPoly(
		mask,
		tri_interp_short,
		cv::Scalar(1.0, 1.0, 1.0, 1.0)
	);

	img_trans.copyTo(img_out, mask);
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
