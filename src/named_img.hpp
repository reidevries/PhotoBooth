#ifndef __NAMED_IMG_HPP_
#define __NAMED_IMG_HPP_

#include <opencv2/core.hpp>

struct NamedImg {
	std::string name;
	cv::Mat img;
};

#endif // __NAMED_IMG_HPP_
