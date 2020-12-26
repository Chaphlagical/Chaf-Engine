#include <Canvas/Rectangle.h>

void Chaf::Rectangle::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    if (pointList.size() == 2)
    {
        auto p0 = *pointList[0];
        auto p1 = *pointList[1];
        drawList->AddRect({ origin.x + p0.x, origin.y + p0.y }, { origin.x + p1.x, origin.y + p1.y }, IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), 0.f, 15.f, lineWidth);
        drawList->AddRectFilled({ origin.x + p0.x, origin.y + p0.y }, { origin.x + p1.x, origin.y + p1.y }, IM_COL32(fillColor.r * 255.f, fillColor.g * 255.f, fillColor.b * 255.f, fillColor.a * 255.f), 0.f, 15.f);
    }
}

Chaf::Primitive* Chaf::Rectangle::Update(const glm::vec2& pos)
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
