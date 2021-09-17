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
		Ref<Image> m_source = nullptr;
		Ref<Image> m_target = nullptr;
	};
}