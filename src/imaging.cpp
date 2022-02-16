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
	auto proc = img;

	static const cv::Scalar WHITE = cv::Scalar(255, 255, 255);

	auto size = img.size();
	static const int d = 10;
	static const int w = 40;
	static const cv::LineTypes l_t = cv::LINE_AA;
	auto l_w = size.height/w;
	auto x_d = size.width/d;
	auto y_d = size.height/d;

	auto p_tl = cv::Point(x_d, y_d);
	auto p_tl_b = p_tl + cv::Point(0, y_d);
	auto p_tl_r = p_tl + cv::Point(x_d, 0);
	cv::line(proc, p_tl, p_tl_b, WHITE, l_w, l_t);
	cv::line(proc, p_tl, p_tl_r, WHITE, l_w, l_t);

	auto p_tr = cv::Point((d-1)*x_d, y_d);
	auto p_tr_b = p_tr + cv::Point(0, y_d);
	auto p_tr_l = p_tr - cv::Point(x_d, 0);
	cv::line(proc, p_tr, p_tr_b, WHITE, l_w, l_t);
	cv::line(proc, p_tr, p_tr_l, WHITE, l_w, l_t);

	auto p_bl = cv::Point(x_d, (d-1)*y_d);
	auto p_bl_t = p_bl - cv::Point(0, y_d);
	auto p_bl_r = p_bl + cv::Point(x_d, 0);
	cv::line(proc, p_bl, p_bl_t, WHITE, l_w, l_t);
	cv::line(proc, p_bl, p_bl_r, WHITE, l_w, l_t);

	auto p_br = cv::Point((d-1)*x_d, (d-1)*y_d);
	auto p_br_t = p_br - cv::Point(0, y_d);
	auto p_br_l = p_br - cv::Point(x_d, 0);
	cv::line(proc, p_br, p_br_t, WHITE, l_w, l_t);
	cv::line(proc, p_br, p_br_l, WHITE, l_w, l_t);

	auto out = proc;
	cv::repeat(proc, 2, 2, out);
	return out;
}
