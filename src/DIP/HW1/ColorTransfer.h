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
	};
}