#include <Canvas/Polygon.h>

void Chaf::Polygon::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    std::vector<ImVec2> res;
    for (auto point : pointList)
    {
        res.push_back({ (*point).x + origin.x, (*point).y + origin.y });
    }
    if (end)
    {
        drawList->AddConvexPolyFilled(res.data(), res.size(), IM_COL32(fillColor.r * 255.f, fillColor.g * 255.f, fillColor.b * 255.f, fillColor.a * 255.f));
        drawList->AddPolyline(res.data(), res.size(), IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), true, lineWidth);
    }
    else
    {
        drawList->AddPolyline(res.data(), res.size(), IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), false, lineWidth);
    }
}

Chaf::Primitive* Chaf::Polygon::Update(const glm::vec2& pos)
{
    if (start && pos == *pointList[0])
    {
        end = true;
        pointList.push_back(pointList[0]);
        return this;
    }
    if (!start)start = true;
    pointList.push_back(new glm::vec2(pos));
    return this;
}
