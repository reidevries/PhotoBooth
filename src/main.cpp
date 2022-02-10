#include "numbers.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>

#include "app_gui.hpp"
#include "app_type.hpp"
#include "app_process.hpp"

int main(int argc, char** argv)
{
	std::vector<NamedImg> images;

	auto img_list_filename = std::string("img_list.txt");
	auto app_type = app::Process;

	auto img_filename = std::string("test.jpg");
	auto output_folder_filename = std::string("output");

	if (argc > 1) {
		if (strncmp("morph", argv[1], 5) == 0) {
			app_type = app::GuiMorph;
		} else if (strncmp("average", argv[1], 5) == 0) {
			app_type = app::GuiAverage;
		} else if (strncmp("process", argv[1], 4) == 0) {
			app_type = app::Process;
		} else {
			std::cout << "argument " << argv[1] << " invalid" << std::endl;
		}
		if (argc > 2) {
			if (app_type != app::Process) {
				img_list_filename = argv[2];
			} else {
				if (argc < 3) {
					std::cout << "too few args, expected an output folder name"
						<< std::endl;
				} else {
					img_filename = argv[3];
				}
				if (argc > 4) {
					output_folder_filename = argv[4];
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
			output_folder_filename
		);
		break;
	}
	}
	return 0;
}
