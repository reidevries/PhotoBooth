#include "imaging.hpp"

auto imaging::load_img_and_process(
	const std::string& filename,
	const cv::Size& expected_size,
	const int rotation
) -> NamedImg
{
	const float ASPECT_RATIO = 3.0/3.0;
	const float MAX_HEIGHT = 500;

	auto img = cv::imread(filename, cv::IMREAD_COLOR);

	// rotate by 90 degrees as many times as specified
	// rotation=0 doesn't rotate, rotation=1 rotates 90 degrees clockwise, etc
	if (rotation%4 > 0) {
		auto amount = cv::ROTATE_90_CLOCKWISE;
		if (rotation%4 == 2) amount = cv::ROTATE_180;
		else if (rotation%4 == 3) amount = cv::ROTATE_90_COUNTERCLOCKWISE;
		cv::rotate(img, img, amount);
	}

	// crop to 3:4 aspect ratio if possible
	auto new_height = img.size().width/ASPECT_RATIO;
	if (new_height < img.size().height) {
		auto crop_y = (img.size().height - new_height)/2;
		img = img.rowRange(crop_y, img.size().height-crop_y);
	} else {
		auto new_width = img.size().height*ASPECT_RATIO;
		auto crop_x = (img.size().width - new_width)/2;
		img = img.colRange(crop_x, img.size().width-crop_x);
	}

	// calculate a new size less than MAX_HEIGHT tall 
	auto denom = static_cast<int>(std::ceil(img.size().height/MAX_HEIGHT));
	auto new_size = cv::Size(img.size().width/denom, img.size().height/denom);
	// check if the resized image is the expected side
	if (new_size != expected_size && expected_size != cv::Size(0,0)) {
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

auto imaging::process_aly_style(
	const cv::Mat& img,
	const std::string& top_text,
	const std::string& bottom_text
) -> cv::Mat
{
	// probably don't need to load the font every time, can just pass as arg
	auto font = cv::freetype::createFreeType2();
	font->loadFontData("OCRA.ttf", 0);
	
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

	auto larger_text_w = std::max(top_text.size(), bottom_text.size());
	auto char_w_target = (p_br.x - p_bl.x - l_w*2)/larger_text_w;
	auto char_h = char_w_target*1.4;
	std::cout << "char height calculated as " << char_h << std::endl;

	auto top_size = font->getTextSize(
		top_text,
		char_h,
		1.5,
		0
	);
	auto bottom_size = font->getTextSize(
		bottom_text,
		char_h,
		1.5,
		0
	);
	std::cout << "bottom width is " << bottom_size.width << std::endl;

	auto top_bl = cv::Point(
		(size.width - top_size.width)/2,
		p_tl.y + top_size.height + 2*l_w
	);
	auto bottom_bl = cv::Point(
		(size.width - bottom_size.width)/2,
		p_bl.y - 2*l_w
	);
	// outline
	font->putText(
		proc,
		top_text,
		top_bl,
		char_h,
		BLACK,
		3,
		l_t,
		true
	);
	// main text
	font->putText(
		proc,
		top_text,
		top_bl,
		char_h,
		GREEN,
		-1,
		l_t,
		true
	);
	// outline
	font->putText(
		proc,
		bottom_text,
		bottom_bl,
		char_h,
		BLACK,
		3,
		l_t,
		true
	);
	// main text
	font->putText(
		proc,
		bottom_text,
		bottom_bl,
		char_h,
		GREEN,
		-1,
		l_t,
		true
	);

	auto out = proc;
	cv::repeat(proc, 3, 1, out);
	return out;
}

auto imaging::process_aly_style(
	const cv::Mat& img,
	const Config& config
) -> cv::Mat
{
	auto top_text = config.get_top_text();
	auto bottom_text = config.get_bottom_text();
	if (top_text == "") {
		top_text = get_cur_time_str();
	}
	if (bottom_text == "") {
		bottom_text = "Kaurna Land";
	}
	return process_aly_style(
		img,
		top_text,
		bottom_text
	);
}

auto imaging::process_aly_style(
	const cv::Mat& img
) -> cv::Mat
{
	return process_aly_style(
		img,
		get_cur_time_str(),
		"Kaurna Land"
	);
}
