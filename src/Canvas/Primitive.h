#pragma once

#include <glm/glm.hpp>

#include <vector>

#include <imgui.h>

namespace Chaf
{
	struct Primitive
	{
	public:
		virtual void Draw(ImDrawList* drawList, const glm::vec2& origin) = 0;
		virtual Primitive* Update(const glm::vec2& pos) = 0;
		virtual ~Primitive();
	protected:
		void DrawPoint(ImDrawList* drawList, const glm::vec2& origin);
	public:
		std::vector<glm::vec2*> pointList;
		glm::vec4 lineColor{ 0.f, 1, 0.f, 1.f };
		float lineWidth { 3.f };
		glm::vec4 pointColor{ 0.f, 0.f, 1.f, 1.f };
		float pointSize{5.f};
		glm::vec4 fillColor{0.f, 0.f, 0.f, 0.f};
		bool start{ false };
		bool end{ false };
	};
}