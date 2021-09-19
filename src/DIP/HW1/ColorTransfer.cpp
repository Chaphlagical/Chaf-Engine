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

		// Convert to double
		cv::Mat source;
		m_source->getImage().convertTo(source, CV_64FC3);

		cv::Mat target;
		m_target->getImage().convertTo(target, CV_64FC3);

		// Convert to l-alpha-beta color space
		source.forEach<cv::Vec3d>([this](cv::Vec3d& pixel, const int* position) {
			pixel = RGB2lab(pixel);
			});

		cv::Vec3d source_mean, source_stddev;
		cv::meanStdDev(source, source_mean, source_stddev);

		target.forEach<cv::Vec3d>([this](cv::Vec3d& pixel, const int* position) {
			pixel = RGB2lab(pixel);
			});

		cv::Vec3d target_mean, target_stddev;
		cv::meanStdDev(target, target_mean, target_stddev);

		// Color transfer
		source.forEach<cv::Vec3d>([this, target_stddev, source_stddev, source_mean, target_mean](cv::Vec3d& pixel, const int*) {
			cv::multiply(target_stddev, pixel - source_mean, pixel);
			cv::divide(pixel, source_stddev, pixel);
			cv::add(pixel, target_mean, pixel);
			pixel = lab2RGB(pixel);
			});

		// Convert to uint
		source.convertTo(source, CV_8UC3);

		return CreateRef<Image>(source);
	}

	cv::Vec3d ColorTransfer::RGB2lab(const cv::Vec3d& rgb)
	{
		cv::Vec3d LMS = cv::Matx33d(
			0.3811, 0.5783, 0.0402,
			0.1967, 0.7244, 0.0782,
			0.0241, 0.1288, 0.844) * rgb;

		LMS.val[0] = LMS.val[0] == 0.0 ? 1.0 : LMS.val[0];
		LMS.val[1] = LMS.val[1] == 0.0 ? 1.0 : LMS.val[1];
		LMS.val[2] = LMS.val[2] == 0.0 ? 1.0 : LMS.val[2];

		cv::log(LMS, LMS);

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