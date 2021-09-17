#pragma once

#include "ColorEditing.h"

#include <opencv2/opencv.hpp>

namespace Chaf
{
	class ColorTransfer :public ColorEditing
	{
	public:
		ColorTransfer(const Ref<Image>& source, const Ref<Image>& target);

		~ColorTransfer() = default;

		virtual Ref<Image> solve() override;

	private:
		// RGB to l alpha beta
		cv::Vec3d RGB2lab(const cv::Vec3d& rgb);

		// l alpha beta to RGB
		cv::Vec3d lab2RGB(const cv::Vec3d& lab);
	};
}