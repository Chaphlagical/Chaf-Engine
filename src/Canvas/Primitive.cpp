#include <Canvas/Primitive.h>

Chaf::Primitive::~Primitive()
{
	for (auto* point : pointList)
		delete point;
}

void Chaf::Primitive::DrawPoint(ImDrawList* drawList, const glm::vec2& origin)
{
	for (auto point : pointList)
	{
		drawList->AddCircleFilled(ImVec2(origin.x + (*point).x, origin.y + (*point).y), pointSize, IM_COL32(pointColor.r * 255.f, pointColor.g * 255.f, pointColor.b * 255.f, pointColor.a * 255.f), pointSize * 2);
	}
}
