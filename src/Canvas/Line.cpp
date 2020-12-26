#include <Canvas/Line.h>

void Chaf::Line::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    if (pointList.size() == 2)
        drawList->AddLine({ origin.x + (*pointList[0]).x, origin.y + (*pointList[0]).y }, { origin.x + (*pointList[1]).x, origin.y + (*pointList[1]).y }, IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), lineWidth);
}

Chaf::Primitive* Chaf::Line::Update(const glm::vec2& pos)
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
