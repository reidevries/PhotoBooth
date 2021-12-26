#include "numbers.hpp"
#include <opencv2/highgui.hpp>

#include "gui.hpp"

int main()
{
	std::vector<cv::Mat> images;

	try {
		utils::read_img_list("img_list.txt", images);
	} catch (const cv::Exception& e) {
		std::cerr << "Error opening img list: " << e.msg << std::endl;
		exit(1);
	}
	std::cout << "loaded " << images.size() << " images from img_list.txt"
		<< std::endl;

	//initialize FaceMorpher gui
	auto gui = FaceMorpher(images);

    // display until user presses q
    while(cv::waitKey() != 'q') {}
	return 0;
}
