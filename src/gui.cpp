#include "gui.hpp"

using namespace gui;

std::string FaceAverager::window_name = "face averager";

void FaceAverager::trackbar_callback(int pos, void* ptr)
{
	auto p = static_cast<GuiParams*>(ptr);
	cv::imshow(window_name, p->avg_images[pos]);
}

FaceAverager::FaceAverager(const std::vector<NamedImg>& images) : params(images)
{
	auto face_averager = face::FaceAverager();
	std::cout << "averaging faces, pls wait" << std::endl;
	for (auto& img : images) {
		auto face = face::Face(img, params.face_detector);
		params.avg_images.push_back(face_averager.push(img.img, face));
	}
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	cv::createTrackbar(
		"position",
		window_name,
		NULL,
		images.size()-1,
		trackbar_callback,
		static_cast<void*>(&params)
	);
}


std::string FaceMorpher::window_name = "face morpher";

FaceMorpher::FaceMorpher(const std::vector<NamedImg>& images) : params(images)
{
	params.img1 = images[0];
	params.img2 = images[1];
	params.face1 = face::Face(params.img1, params.face_detector);
	params.face2 = face::Face(params.img2, params.face_detector);

	cv::namedWindow(window_name, cv::WINDOW_NORMAL);

	cv::createTrackbar(
		"position",
		window_name,
		NULL,
		10,
		trackbar_callback,
		static_cast<void*>(&params)
	);
	cv::createTrackbar(
		"face1",
		window_name,
		NULL,
		images.size()-1,
		select_face1_callback,
		static_cast<void*>(&params)
	);
	cv::createTrackbar(
		"face2",
		window_name,
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
	p->face1 = face::Face(p->img1, p->face_detector);
	for (u8 i = 0; i < 11; ++i) {
		p->faces[i] = morph::warp_face_fading(
			p->img1.img,
			p->img2.img,
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
	p->face2 = face::Face(p->img2, p->face_detector);
	for (u8 i = 0; i < 11; ++i) {
		p->faces[i] = p->img2.img.clone();
		p->face2.draw_markers(p->faces[i]);
		/*morph::warp_face_fading(
			p->img1,
			p->img2,
			p->face1,
			p->face2,
			i/10.0
		);*/
	}
}
void FaceMorpher::trackbar_callback(int pos, void* ptr)
{
	auto p = static_cast<GuiParams*>(ptr);
	cv::imshow(window_name, p->faces[pos]);
}
