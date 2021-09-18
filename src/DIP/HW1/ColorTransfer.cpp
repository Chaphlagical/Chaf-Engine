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
}