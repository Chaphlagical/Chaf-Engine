#pragma once

#include <vector>

#include <Canvas/Primitive.h>
#include <imgui.h>

namespace Chaf
{
	class Canvas
	{
	public:
		void Draw(ImDrawList* drawList, const glm::vec2& origin);
		bool IsFinish();
		Primitive* GetLast();
		void AddShape(Primitive* primitive);
		Primitive* GetShape(glm::vec2* point);
		void Erase(glm::vec2* point);
		void Clear();
		glm::vec2* Pick(const glm::vec2& pos);
	private:
		std::vector<Primitive*> m_shapes;
	};
}
