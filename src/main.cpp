#include "numbers.hpp"
#include "named_img.hpp"
#include <opencv2/highgui.hpp>

#include "gui.hpp"

int main(int argc, char** argv)
{
	std::vector<NamedImg> images;

	auto img_list_filename = std::string("img_list.txt");
	auto gui_type = gui::None;

	auto img_filename = std::string("test.jpg");
	auto avg_img_filename = std::string("test.jpg");
	auto avg_face_filename = std::string("");

	if (argc > 1) {
		if (strncmp("morph", argv[1], 5) == 0) {
			gui_type = gui::Morph;
		} else if (strncmp("average", argv[1], 5) == 0) {
			gui_type = gui::Average;
		} else {
			std::cout << "argument " << argv[1] << " invalid" << std::endl;
		}
		if (argc > 2) {
			if (gui_type != gui::None) {
				img_list_filename = argv[2];
			} else {
				if (argc <= 4) {
					std::cout << "too few args, expected two image filenames"
						<< std::endl;
				} else {
					img_filename = argv[3];
					avg_img_filename = argv[4];
				}
				if (argc > 5) {
					avg_face_filename = argv[5];
				}
			}
		}
	}

	if (gui_type != gui::None) {
		try {
			utils::read_img_list(img_list_filename, images);
		} catch (const cv::Exception& e) {
			std::cerr << "Error opening img list: " << e.msg << std::endl;
			exit(1);
		}
		std::cout << "loaded " << images.size() << " images from "
			<< img_list_filename << std::endl;
	}

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
	case gui::None: {
		face::FaceAverager averager;
		face::FaceDetector detector;
		auto img = NamedImg{
			img_filename,
			cv::imread(img_filename, cv::IMREAD_COLOR)
		};
		auto face = face::Face(img, detector);
		auto avg_img = NamedImg{
			img_filename,
			cv::imread(img_filename, cv::IMREAD_COLOR)
		};
		auto avg_face = face::Face();
		if (avg_face_filename.size() > 0) {
			avg_face = face::Face::load(avg_face_filename);
		} else {
			avg_face = face::Face(avg_img, detector);
		}
		auto num_faces = utils::load_num_faces();
		auto new_avg = averager.process(
			img.img,
			face,
			avg_img.img,
			avg_face,
			num_faces
		);

		break;
	}
	}
	return 0;
}
