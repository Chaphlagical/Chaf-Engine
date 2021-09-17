#include "ColorTransfer.h"

namespace Chaf
{
	ColorTransfer::ColorTransfer(const Ref<Image>& source, const Ref<Image>& target) :
		ColorEditing(source, target)
	{
		RGB2lab({ 1,2,3 });
	}

	Ref<Image> ColorTransfer::solve()
	{
		CHAF_ASSERT(m_source->getImage().type() == CV_8UC3 && "Only support CV_8UC3");

		cv::Mat source;
		m_source->getImage().convertTo(source, CV_64FC3);

		for (auto i = 0; i < source.rows; i++)
		{
			for (auto j = 0; j < source.cols; j++)
			{
				auto& pixel = source.at<cv::Vec3d>(i, j);
				pixel = RGB2lab(pixel);
				//std::cout << pixel << std::endl;
			}
		}

		cv::Scalar lmean, stddev;
		/*auto lmean = cv::mean(l);
		auto amean = cv::mean(a);
		auto bmean = cv::mean(b);*/

		cv::meanStdDev(source, lmean, stddev);
		

		for (auto i = 0; i < source.rows; i++)
		{
			for (auto j = 0; j < source.cols; j++)
			{
				auto& pixel = source.at<cv::Vec3d>(i, j);
				pixel = lab2RGB(pixel);
			}
		}

		source.convertTo(source, CV_8UC3);

		return CreateRef<Image>(source);
	}

	cv::Vec3d ColorTransfer::RGB2lab(const cv::Vec3d& rgb)
	{
		cv::Vec3d LMS = cv::Matx33d(
			0.3811, 0.5783, 0.0402,
			0.1967, 0.7244, 0.0782,
			0.0241, 0.1288, 0.844) * rgb;
		cv::log(
			cv::Matx33d(
				0.3811, 0.5783, 0.0402,
				0.1967, 0.7244, 0.0782,
				0.0241, 0.1288, 0.844) * rgb, LMS);

		LMS /= log(10);

		return
			cv::Matx33d(
				1 / std::sqrt(3.0), 0.0, 0.0,
				0.0, 1 / std::sqrt(6.0), 0.0,
				0.0, 0.0, 1 / std::sqrt(2.0)) *
			cv::Matx33d(
				1.0, 1.0, 1.0,
				1.0, 1.0, -2.0,
				1.0, -1.0, 0.0) *
			LMS;
	}

	cv::Vec3d ColorTransfer::lab2RGB(const cv::Vec3d& lab)
	{
		cv::Vec3d LMS =
			cv::Matx33d(
				1.0, 1.0, 1.0,
				1.0, 1.0, -1.0,
				1.0, -2.0, 0.0) *
			cv::Matx33d(
				std::sqrt(3.0) / 3.0, 0.0, 0.0,
				0.0, std::sqrt(6.0) / 6.0, 0.0,
				0.0, 0.0, std::sqrt(2.0) / 2.0) * lab;

		LMS = cv::Vec3d(std::pow(10, LMS.val[0]), std::pow(10, LMS.val[1]), std::pow(10, LMS.val[2]));

		return cv::Matx33d(4.4679, -3.5873, 0.1193, -1.2186, 2.3809, -0.1624, 0.0497, -0.2439, 1.2045) * LMS;
	}
}