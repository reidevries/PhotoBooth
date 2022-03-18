#include "imaging.hpp"

auto imaging::load_img_and_process(
	const std::string& filename,
	const cv::Size& expected_size
) -> NamedImg
{
	const float ASPECT_RATIO = 5.0/4.0;
	const float MAX_HEIGHT = 400;

	auto img = cv::imread(filename, cv::IMREAD_COLOR);
	
	// crop to 5:4 aspect ratio if possible
	auto new_height = img.size().width*ASPECT_RATIO;
	if (new_height < img.size().height) {
		auto crop_y = (img.size().height - new_height)/2;
		img = img.rowRange(crop_y, img.size().height-crop_y);
	} else {
		auto new_width = img.size().height/ASPECT_RATIO;
		auto crop_x = (img.size().width - new_width)/2;
		img = img.colRange(crop_x, img.size().width-crop_x);
	}

	// calculate a new size less than 400px tall 
	auto denom = static_cast<int>(std::ceil(img.size().height/MAX_HEIGHT));
	auto new_size = cv::Size(img.size().width/denom, img.size().height/denom);
	// check if the resized image is the expected side
	if (new_size != expected_size) {
		std::cout << "hmm, new size is " << new_size
			<< " but expected size was " << expected_size << std::endl;
		new_size = expected_size;
	}
	// try to resize image
	try {
		cv::resize(img, img, new_size, cv::INTER_LINEAR);
		std::cout << "resized to " << new_size << std::endl;
	} catch (const cv::Exception& e) {
		std::cout << "got error while resizing: " << e.msg << std::endl;
	}

	return NamedImg{ filename, img };
}

auto get_cur_time_str() -> std::string
{
	time_t cur_time;
	time(&cur_time);
	auto local_time = localtime(&cur_time);
	int y = local_time->tm_year - 100;
	int mo = local_time->tm_mon + 1;
	int d = local_time->tm_mday;
	int h = local_time->tm_hour;
	int mi = local_time->tm_min;
	int s = local_time->tm_sec;

	std::stringstream buf;
	// not sure why but seems like i need to set these before every
	// int for some reason
	buf << std::setfill('0') << std::setw(2)
		<< y << ":"
		<< std::setfill('0') << std::setw(2)
		<< mo << ":"
		<< std::setfill('0') << std::setw(2)
		<< d << ":"
		<< std::setfill('0') << std::setw(2)
		<< h << ":"
		<< std::setfill('0') << std::setw(2)
		<< mi << ":"
		<< std::setfill('0') << std::setw(2)
		<< s;
	std::cout << "got current time as " << buf.str() << std::endl;
	return buf.str();
}

auto imaging::process_aly_style(const cv::Mat& img) -> cv::Mat
{
	auto proc = img.clone();

	static const cv::Scalar WHITE = cv::Scalar(255, 255, 255);
	static const cv::Scalar BLACK = cv::Scalar(0, 0, 0);
	static const cv::Scalar GREEN = cv::Scalar(0,255,0);

	auto size = img.size();
	static const int d = 10;
	static const int w = 60;
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

	auto cur_time_str = get_cur_time_str();
	auto time_size = cv::getTextSize(
		cur_time_str,
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		1.5,
		0
	);
	auto loc_size = cv::getTextSize(
		"Kaurna Land",
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		1.5,
		0
	);
	auto time_bl = cv::Point(
		(size.width - time_size.width)/2,
		p_tl.y + time_size.height + 2*l_w
	);
	auto loc_bl = cv::Point(
		(size.width - loc_size.width)/2,
		p_bl.y - 2*l_w
	);
	// outline
	cv::putText(
		proc,
		get_cur_time_str(),
		time_bl,
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		BLACK,
		2,
		l_t
	);
	// main text
	cv::putText(
		proc,
		get_cur_time_str(),
		time_bl,
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		GREEN,
		1.5,
		l_t
	);
	// outline
	cv::putText(
		proc,
		"Kaurna Land",
		loc_bl,
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		BLACK,
		2,
		l_t
	);
	// main text
	cv::putText(
		proc,
		"Kaurna Land",
		loc_bl,
		cv::FONT_HERSHEY_DUPLEX,
		0.7,
		GREEN,
		1.5,
		l_t
	);

	auto out = proc;
	cv::repeat(proc, 4, 1, out);
	return out;
}
