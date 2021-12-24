#include "utils.hpp"

void utils::read_img_list(
	const std::string& filename,
	std::vector<cv::Mat>& images
)
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) {
		CV_Error(cv::Error::StsBadArg, "invalid input filename");
	}
	std::string line;
	while (std::getline(file, line)) {
		images.push_back(cv::imread(line, cv::IMREAD_COLOR));
	}
}

auto utils::reshape_img_to_row(const cv::Mat& img) -> cv::Mat
{
	// split individual channels from img
	cv::Mat row_channels[3];
	cv::split(img.clone(), row_channels);

	// append each channel into the same grayscale Mat
	// then put them all on the same row
	auto output = cv::Mat();
	output.push_back(row_channels[0]);
	output.push_back(row_channels[1]);
	output.push_back(row_channels[2]);
	output = output.reshape(0,1);
	return output;
}

auto utils::reshape_images_to_rows(const std::vector<cv::Mat> &images)
	-> cv::Mat
{
	auto img_size = 3*images[0].rows*images[0].cols;
	auto result = cv::Mat(
		static_cast<int>(images.size()),
		img_size,
		CV_32F
	);
	for (u64 i = 0; i < images.size(); ++i) {
		auto row_i = result.row(i);
		utils::reshape_img_to_row(images[i]).convertTo(row_i, CV_32F);
	}
	return result;
}

auto utils::reshape_row_to_img(const cv::Mat& row, const u64 img_rows)
	-> cv::Mat
{
	// split row up into three equal parts, and place them as rows of a matrix
	auto input = row.reshape(0,3);
	cv::Mat channels[3];
	for (u8 i = 0; i < 3; ++i) {
		input.row(i).reshape(0, img_rows).convertTo(channels[i], CV_32F);
	}
	cv::Mat output;
	cv::merge(channels, 3, output);
	return output;
}

auto utils::reshape_row_to_raw_grayscale_images(
	const cv::Mat& row,
	const u64 img_rows
) -> cv::Mat
{
	return row.reshape(0, img_rows*3);
}

void utils::normalize_default(cv::Mat& mat)
{
    cv::normalize(mat, mat, 0, 255, cv::NORM_MINMAX, CV_8UC3);
}

auto utils::scale_point_to_other_rect(
	const cv::Point2f& p,
	const cv::Rect& src,
	const cv::Rect& dst
) -> cv::Point2f
{
	auto x_scale = dst.width/src.width;
	auto y_scale = dst.height/src.height;
	return cv::Point2f(
		x_scale*(p.x - src.x) + dst.x,
		y_scale*(p.y - src.y) + dst.y
	);
}
