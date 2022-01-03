#include "numbers.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>

#include "gui.hpp"

int main(int argc, char** argv)
{
	std::vector<NamedImg> images;

	auto img_list_filename = std::string("img_list.txt");
	auto gui_type = gui::Morph;

	if (argc > 0) {
		img_list_filename = argv[1];
		if (argc > 1) {
			if (strncmp("morph", argv[2], 5) == 0) {
				gui_type = gui::Morph;
			} else if (strncmp("average", argv[2], 5) == 0) {
				gui_type = gui::Average;
			} else {
				std::cout << "argument " << argv[2] << " invalid" << std::endl;
			}
		}
	}

	try {
		utils::read_img_list(img_list_filename, images);
	} catch (const cv::Exception& e) {
		std::cerr << "Error opening img list: " << e.msg << std::endl;
		exit(1);
	}
	std::cout << "loaded " << images.size() << " images from "
		<< img_list_filename << std::endl;

	//initialize gui
	switch (gui_type) {
	case gui::Morph: {
		auto gui = gui::FaceMorpher(images);
		while(cv::waitKey() != 'q') {}
		break;
	}
	case gui::Average: {
		auto gui = gui::FaceAverager(images);
		while(cv::waitKey() != 'q') {}
		break;
	}
	}
	return 0;
}
