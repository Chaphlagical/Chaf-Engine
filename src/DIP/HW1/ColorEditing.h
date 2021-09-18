#pragma once

#include <DIP/DIP/Image.h>

#include <Engine/core.h>

namespace Chaf
{
	class ColorEditing
	{
	public:
		ColorEditing(const Ref<Image>& source, const Ref<Image>& target);

		~ColorEditing() = default;

		virtual Ref<Image> solve() = 0;

	protected:
		// RGB to l alpha beta
		cv::Vec3d RGB2lab(const cv::Vec3d& rgb);

		// l alpha beta to RGB
		cv::Vec3d lab2RGB(const cv::Vec3d& lab);

	protected:
		Ref<Image> m_source = nullptr;
		Ref<Image> m_target = nullptr;
	};
}