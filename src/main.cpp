#include "numbers.hpp"
#include <opencv2/highgui.hpp>

#include "pca.hpp"
#include "utils.hpp"
#include "face.hpp"

struct trackbar_params
{
	std::vector<cv::Mat> images;
    cv::Mat data;
    int img_xsize;
    PCA pca;
    std::string window_name;
	Face face_src, face_dst;
};

void trackbar_callback(int pos, void* ptr)
{
    struct trackbar_params *p = (struct trackbar_params *)ptr;
	/*
	cv::Mat img = utils::reshape_row_to_img(
		p->pca.get_component_range(fmax(0,pos-7), pos),
		p->img_xsize
	);
	utils::normalize_default(img);
	*/

	//cv::imshow(p->window_name, img);
}

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

	auto data = utils::reshape_images_to_rows(images);

	auto face_detector = FaceDetector("shape_predictor_68_face_landmarks.dat");

	// init highgui window
	auto window_name = "reconstruction";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	// trackbar_params struct to pass to the trackbar handler
    trackbar_params p;
	p.images = images;
    p.data = data;
    p.img_xsize = images[0].rows;
    p.pca = PCA(data);
    p.window_name = window_name;
	p.face_src = face_detector.get_landmarks(images[0]);
	p.face_dst = face_detector.get_landmarks(images[1]);
    // create the tracbar
    cv::createTrackbar(
		"component",
		window_name,
		NULL,
		100,
		trackbar_callback,
		(void*)&p
	);
    // display until user presses q
    while(cv::waitKey() != 'q') {}
	return 0;
}
