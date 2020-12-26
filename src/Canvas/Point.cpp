#include "Point.h"

void Chaf::Point::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
}

Chaf::Primitive* Chaf::Point::Update(const glm::vec2& pos)
{
    start = true;
    pointList.push_back(new glm::vec2(pos));
    end = true;
    return this;
}
