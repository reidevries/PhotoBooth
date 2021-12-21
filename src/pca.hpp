#ifndef __PCA_HPP_
#define __PCA_HPP_

#include "numbers.hpp"
#include <opencv2/core.hpp>

class PCA
{
	cv::Mat means;
	cv::Mat eigenvecs;
	cv::Mat eigenvals;
public:
	PCA() {}
	PCA(const cv::Mat& data);

	void train(const cv::Mat& data);

	auto project(const cv::Mat& data_row) const -> cv::Mat;
	auto project(const cv::Mat& data_row, const int num_components) const
		-> cv::Mat;
	auto reconstruct(const cv::Mat& input) const -> cv::Mat;
	auto reconstruct(const cv::Mat& input, const int num_components) const
		->cv::Mat;

	auto get_component(const int index) -> cv::Mat;
	auto get_component_range(const int start, const int end) -> cv::Mat;
};

#endif // __PCA_HPP_
