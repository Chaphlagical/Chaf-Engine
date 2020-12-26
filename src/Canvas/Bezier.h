#pragma once

#include <Canvas/Primitive.h>

namespace Chaf
{
	struct Bezier : public Primitive
	{
		virtual void Draw(ImDrawList* drawList, const glm::vec2& origin) override;
		virtual Primitive* Update(const glm::vec2& pos) override;
	};
}