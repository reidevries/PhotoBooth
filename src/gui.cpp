#include "gui.hpp"

using namespace gui;




std::string FaceMorpher::window_name = "face morpher";

FaceMorpher::FaceMorpher(
	const std::vector<cv::Mat>& images
) : params(images)
{
	params.img1 = images[0];
	params.img2 = images[0];

	cv::namedWindow(FaceMorpher::window_name, cv::WINDOW_NORMAL);

	cv::createTrackbar(
		"position",
		FaceMorpher::window_name,
		NULL,
		10,
		trackbar_callback,
		static_cast<void*>(&params)
	);
	cv::createTrackbar(
		"face1",
		FaceMorpher::window_name,
		NULL,
		images.size()-1,
		select_face1_callback,
		static_cast<void*>(&params)
	);
	cv::createTrackbar(
		"face2",
		FaceMorpher::window_name,
		NULL,
		images.size()-1,
		select_face2_callback,
		static_cast<void*>(&params)
	);
}

void FaceMorpher::select_face1_callback(int pos, void* ptr)
{
	auto p = static_cast<GuiParams*>(ptr);
	p->img1 = p->images[pos];
	p->face1 = Face(p->img1, p->face_detector);
	for (u8 i = 0; i < 11; ++i) {
		p->faces[i] = morph::warp_face_fading(
			p->img1,
			p->img2,
			p->face1,
			p->face2,
			i/10.0
		);
	}
}
void FaceMorpher::select_face2_callback(int pos, void* ptr)
{
	auto p = static_cast<GuiParams*>(ptr);
	p->img2 = p->images[pos];
	p->face2 = Face(p->img2, p->face_detector);
	for (u8 i = 0; i < 11; ++i) {
		p->faces[i] = morph::warp_face_fading(
			p->img1,
			p->img2,
			p->face1,
			p->face2,
			i/10.0
		);
	}
}
void FaceMorpher::trackbar_callback(int pos, void* ptr)
{
	auto p = static_cast<GuiParams*>(ptr);
	cv::imshow(FaceMorpher::window_name, p->faces[pos]);
}
