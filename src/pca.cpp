#include "pca.hpp"

PCA::PCA(const cv::Mat& data)
{
	train(data);
}

void PCA::train(const cv::Mat& data)
{
	cv::PCACompute(
		data,
		means,
		eigenvecs,
		eigenvals,
		static_cast<int>(data.rows)
	);
}

auto PCA::project(const cv::Mat& data_row) const -> cv::Mat
{
	cv::Mat output;
	cv::PCAProject(data_row, means, eigenvecs, output);
	return output;
}

auto PCA::project(const cv::Mat& data_row, const int num_components) const
	-> cv::Mat
{
	auto z = eigenvecs.rowRange(0, num_components);
	cv::Mat output;
	cv::PCAProject(data_row, means, z, output);
	return output;
}

auto PCA::reconstruct(const cv::Mat& input) const -> cv::Mat
{
	cv::Mat output;
	cv::PCABackProject(input, means, eigenvecs, output);
	return output;
}

auto PCA::reconstruct(const cv::Mat& input, const int num_components) const
	-> cv::Mat
{

	auto z = eigenvecs.rowRange(0, num_components);
	cv::Mat output;
	cv::PCABackProject(input, means, z, output);
	return output;
}

auto PCA::get_component(const int index) -> cv::Mat
{
	return eigenvecs.row(index);
}

auto PCA::get_component_range(const int start, const int end) -> cv::Mat
{
	auto sum = cv::Mat::zeros(1, eigenvecs.cols, eigenvecs.type());
	for (u64 i = start; i < end; ++i) {
		auto component = eigenvecs.row(i);//*eigenvals.at<float>(i);
		sum = sum + component;
	}
	return sum;
}
