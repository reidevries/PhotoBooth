#include "numbers.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>

#include "gui.hpp"

int main(int argc, char** argv)
{
	std::vector<NamedImg> images;

	try {
		utils::read_img_list("img_list.txt", images);
	} catch (const cv::Exception& e) {
		std::cerr << "Error opening img list: " << e.msg << std::endl;
		exit(1);
	}
	std::cout << "loaded " << images.size() << " images from img_list.txt"
		<< std::endl;

	//initialize gui
	if (argc > 0) {
		if (strncmp("morph", argv[1], 5) == 0) {
			auto gui = gui::FaceMorpher(images);
		} else if (strncmp("average", argv[1], 5) == 0) {
			auto gui = gui::FaceAverager(images);
		} else {
			std::cout << "argument " << argv[1] << " invalid" << std::endl;
		}
	}
	std::cout << "no argument, assuming FaceMorpher" << std::endl;
	auto gui = gui::FaceMorpher(images);
	while(cv::waitKey() != 'q') {}

	return 0;
}
