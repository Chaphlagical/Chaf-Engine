#include "Colorization.h"

namespace Chaf
{
	Colorization::Colorization(const Ref<Image>& source, const Ref<Image>& target) :
		ColorEditing(source, target)
	{
	}

	void Colorization::setMethod(ColorizationMethod method)
	{
		m_method = method;
	}

	Ref<Image> Colorization::solve()
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
			pixel.val[1] = pixel.val[2] = pixel.val[0];
			});

		target.forEach<cv::Vec3d>([this](cv::Vec3d& pixel, const int* position) {
			pixel = RGB2lab(pixel);
			});



		return CreateRef<Image>(source);
	}
}