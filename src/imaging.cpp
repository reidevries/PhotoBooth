#include "imaging.hpp"

auto imaging::load_img_and_process(
	const std::string& filename,
	const cv::Size& expected_size
) -> NamedImg
{
	auto img = cv::imread(filename, cv::IMREAD_COLOR);

	//shrink img to be less than 400 pixels tall
	auto denom = static_cast<int>(std::ceil(img.size().height/400));
	auto new_size = cv::Size(img.size().width/denom, img.size().height/denom);
	if (new_size != expected_size) {
		std::cout << "hmm, new size is " << new_size
			<< " but expected size was " << expected_size;
		new_size = expected_size;
	}
	cv::resize(img, img, new_size, cv::INTER_LINEAR);

	std::cout << "resized to " << new_size << std::endl;

	return NamedImg{ filename, img };
}

auto imaging::process_aly_style(const cv::Mat& img) -> cv::Mat
{
	auto out = cv::Mat();
	cv::repeat(img, 2, 2, out);
	return out;
}
