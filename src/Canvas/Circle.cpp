#include <Canvas/Circle.h>

#define PI 3.1415926535

void Chaf::Circle::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    if (pointList.size() == 2)
    {
        auto center = *pointList[0];
        auto p1 = *pointList[1];
        float r = glm::distance(center, p1);
        drawList->AddCircleFilled({ origin.x + center.x, origin.y + center.y }, r, IM_COL32(fillColor.r * 255.f, fillColor.g * 255.f, fillColor.b * 255.f, fillColor.a * 255.f), r * 10);
        drawList->AddCircle({ origin.x + center.x, origin.y + center.y }, r, IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), r * 10, lineWidth);
    }
}

Chaf::Primitive* Chaf::Circle::Update(const glm::vec2& pos)
{
    if (start)
    {
        pointList.push_back(new glm::vec2(pos));
        end = true;
    }
    else
    {
        pointList.push_back(new glm::vec2(pos));
        start = true;
    }
    return this;
}
