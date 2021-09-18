#pragma once

#include "ColorEditing.h"

#include <DIP/DIP/Image.h>

#include <Engine/core.h>

namespace Chaf
{
	enum class ColorizationMethod
	{
		Global,
		Swatches
	};

	class Colorization :public ColorEditing
	{
	public:
		Colorization(const Ref<Image>& source, const Ref<Image>& target);

		~Colorization() = default;

		void setMethod(ColorizationMethod method);

		virtual Ref<Image> solve() override;

	private:
		ColorizationMethod m_method = ColorizationMethod::Global;
	};
}