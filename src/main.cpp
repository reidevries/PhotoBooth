#include "numbers.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>
#include <gphoto2/gphoto2.h>

#include "app_gui.hpp"
#include "app_type.hpp"
#include "app_process.hpp"

int main(int argc, char** argv)
{
	std::vector<NamedImg> images;

	auto img_list_filename = std::string("img_list.txt");
	auto app_type = app::Process;

	auto img_filename = std::string("test.jpg");
	auto avg_img_filename = std::string("avg.jpg");
	auto avg_face_filename = std::string("avg.face");

	if (argc > 1) {
		if (strncmp("morph", argv[1], 5) == 0) {
			app_type = app::GuiMorph;
		} else if (strncmp("average", argv[1], 5) == 0) {
			app_type = app::GuiAverage;
		} else if (strncmp("none", argv[1], 4) == 0) {
			app_type = app::Process;
		} else {
			std::cout << "argument " << argv[1] << " invalid" << std::endl;
		}
		if (argc > 2) {
			if (app_type != app::Process) {
				img_list_filename = argv[2];
			} else {
				if (argc < 4) {
					std::cout << "too few args, expected two image filenames"
						<< std::endl;
				} else {
					img_filename = argv[2];
					avg_img_filename = argv[3];
				}
				if (argc > 5) {
					avg_face_filename = argv[4];
				}
			}
		}
	}

	if (app_type == app::GuiMorph || app_type == app::GuiAverage ) {
		try {
			utils::read_img_list(img_list_filename, images);
		} catch (const cv::Exception& e) {
			std::cerr << "Error opening img list: " << e.msg << std::endl;
			exit(1);
		}
		std::cout << "loaded " << images.size() << " images from "
			<< img_list_filename << std::endl;
	}

	//initialize app
	switch (app_type) {
	case app::GuiMorph: {
		auto gui = app::FaceMorpher(images);
		while(cv::waitKey() != 'q') {}
		break;
	}
	case app::GuiAverage: {
		auto gui = app::FaceAverager(images);
		while(cv::waitKey() != 'q') {}
		break;
	}
	case app::Process: {
		app::process_img(
			img_filename,
			avg_img_filename,
			avg_face_filename,
			"num_faces"
		);
		break;
	}
	}
	return 0;
}
