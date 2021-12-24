#include "numbers.hpp"
#include <opencv2/highgui.hpp>

#include "pca.hpp"
#include "utils.hpp"
#include "face.hpp"
#include "morph.hpp"

int FACE_I1 = 2;
int FACE_I2 = 3;

struct trackbar_params
{
	std::vector<cv::Mat> images;
    cv::Mat data;
    int img_xsize;
    PCA pca;
    std::string window_name;
	Face face_src, face_dst;
	FaceDetector* face_detector;
};

void select_face1_callback(int pos, void* ptr)
{
    struct trackbar_params *p = (struct trackbar_params *)ptr;
	FACE_I1 = pos;
	p->face_src = Face(p->images[FACE_I1], *(p->face_detector));
}
void select_face2_callback(int pos, void* ptr)
{
    struct trackbar_params *p = (struct trackbar_params *)ptr;
	FACE_I2 = pos;
	p->face_dst = Face(p->images[FACE_I2], *(p->face_detector));
}


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

	auto posf = pos/100.0;

	auto img = morph::warp_face_fading(
		p->images[FACE_I1],
		p->images[FACE_I2],
		p->face_src,
		p->face_dst,
		posf
	);

	cv::imshow(p->window_name, img);
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

	//auto data = utils::reshape_images_to_rows(images);


	// init highgui window
	auto window_name = "reconstruction";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	// trackbar_params struct to pass to the trackbar handler
    trackbar_params p;
	p.images = images;
    p.img_xsize = images[FACE_I1].rows;
    p.window_name = window_name;
	auto face_detector = FaceDetector("shape_predictor_68_face_landmarks.dat");
	p.face_detector = &face_detector;
    // create the tracbar
    cv::createTrackbar(
		"component",
		window_name,
		NULL,
		100,
		trackbar_callback,
		(void*)&p
	);
	cv::createTrackbar(
		"face1",
		window_name,
		NULL,
		images.size(),
		select_face1_callback,
		(void*)&p
	);
	cv::createTrackbar(
		"face2",
		window_name,
		NULL,
		images.size(),
		select_face2_callback,
		(void*)&p
	);
    // display until user presses q
    while(cv::waitKey() != 'q') {}
	return 0;
}
