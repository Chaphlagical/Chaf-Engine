#include "Colorization.h"

namespace Chaf
{
	inline void debug_show(const std::string& title, const cv::Mat& image)
	{
		cv::Mat result;
		image.convertTo(result, CV_8UC3);
		cv::imshow(title, result);
		//cv::waitKey(10);
	}

	Colorization::Colorization(const Ref<Image>& source, const Ref<Image>& target) :
		ColorEditing(source, target)
	{
	}

	void Colorization::setMethod(ColorizationMethod method)
	{
		m_method = method;
	}

	void Colorization::addRegion(const Region& source, const Region& target)
	{
		m_src_regions.push_back(source);
		m_tar_regions.push_back(target);
	}

	Ref<Image> Colorization::solve()
	{
		CHAF_ASSERT(m_source->getImage().type() == CV_8UC3 && "Only support CV_8UC3");

		// image converting
		cv::Mat csource;
		cv::cvtColor(m_source->getImage(), csource, cv::COLOR_RGB2Lab);
		csource.convertTo(csource, CV_64FC3);
		std::vector<cv::Mat> csource_channels(3);
		cv::split(csource, csource_channels);

		cv::Mat gsource;
		cv::cvtColor(m_source->getImage(), gsource, cv::COLOR_RGB2GRAY);
		cv::cvtColor(gsource, gsource, cv::COLOR_GRAY2RGB);
		gsource.convertTo(gsource, CV_64FC3);

		cv::Mat target;
		cv::cvtColor(m_target->getImage(), target, cv::COLOR_RGB2GRAY);
		target.convertTo(target, CV_64FC1);
		std::vector<cv::Mat> target_channels(1);
		cv::split(target, target_channels);

		auto csource_luminance = luminanceRemap(csource, target);
		auto gsource_luminance = luminanceRemap(gsource, target);
		auto target_luminance = target.clone();

		auto gsource_stddev = stddevNeiborhood(gsource_luminance);
		auto target_stddev = stddevNeiborhood(target_luminance);

		auto result = cv::Mat(target.rows, target.cols, CV_64FC3, 0.0);

		result.forEach<cv::Vec3d>([this, target_luminance, target_stddev, gsource_luminance, gsource_stddev, target, csource_channels](cv::Vec3d& pixel, const int* position) {
			auto match_point = findBestMatch(target_luminance.at<double>(position), target_stddev.at<double>(position), gsource_luminance, gsource_stddev);
			pixel = cv::Vec3d(target.at<double>(position), csource_channels[1].at<double>(match_point.x, match_point.y), csource_channels[2].at<double>(match_point.x, match_point.y));
			});

		result.convertTo(result, CV_8UC3);
		cv::cvtColor(result, result, cv::COLOR_Lab2RGB);

		return CreateRef<Image>(result);
	}

	cv::Mat Colorization::luminanceRemap(const cv::Mat& source, const cv::Mat& target)
	{
		cv::Scalar source_mean, source_stddev;
		cv::Scalar target_mean, target_stddev;
		cv::meanStdDev(source, source_mean, source_stddev);
		cv::meanStdDev(target, target_mean, target_stddev);

		auto result = cv::Mat(source.rows, source.cols, CV_64FC1, 0.0);

		result.forEach<double>([target_stddev, source_stddev, source_mean, target_mean, source](double& pixel, const int* position) {
			pixel = (target_stddev.val[0] / source_stddev.val[0]) * (source.at<cv::Vec3d>(position).val[0] - source_mean.val[0]) + target_mean.val[0];
			});

		return result;
	}

	cv::Mat Colorization::stddevNeiborhood(const cv::Mat& image, size_t neighborhood_size)
	{
		cv::Mat padding;
		cv::copyMakeBorder(image, padding, neighborhood_size / 2, neighborhood_size / 2, neighborhood_size / 2, neighborhood_size / 2, cv::BORDER_CONSTANT, cv::Scalar(0));

		cv::Mat result(image.rows, image.cols, CV_64FC1, 0.0);
		cv::Mat roi(cv::Size(neighborhood_size, neighborhood_size), CV_64FC1);

		auto left = neighborhood_size / 2;
		auto right = neighborhood_size - neighborhood_size / 2;

		for (auto i = left; i < padding.rows - right; i++)
		{
			for (auto j = left; j < padding.cols - right; j++)
			{
				roi = padding(cv::Range(i - left, i + right), cv::Range(j - left, j + right));
				cv::Scalar mean, stddev;
				cv::meanStdDev(roi, mean, stddev);
				result.at<double>(i - left, j - left) = mean.val[0];
			}
		}

		return result;
	}

	cv::Point2i Colorization::findBestMatch(double target_luminance, double target_stddev, const cv::Mat& source_luminance, const cv::Mat& source_stddev)
	{
		double weight_luminance = 0.5;
		double weight_stddev = 1.0 - weight_luminance;
		double min_weight = std::numeric_limits<double>::max();

		cv::Point2i result;

		for (auto i = 0; i < source_luminance.rows; i++)
		{
			for (auto j = 0; j < source_luminance.cols; j++)
			{
				double weight = weight_luminance * (source_luminance.at<double>(i, j) - target_luminance) * (source_luminance.at<double>(i, j) - target_luminance) +
					weight_stddev * (source_stddev.at<double>(i, j) - target_stddev) * (source_stddev.at<double>(i, j) - target_stddev);
				if (min_weight > weight)
				{
					min_weight = weight;
					result = cv::Point2i(i, j);
				}
			}
		}

		return result;
	}
}