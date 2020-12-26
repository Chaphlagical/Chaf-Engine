#include <Canvas/Bezier.h>

void Chaf::Bezier::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    std::vector<ImVec2> res;
    for (auto point : pointList)
    {
        res.push_back({ (*point).x + origin.x, (*point).y + origin.y });
    }
    if(!res.empty())
       drawList->AddPolyline(res.data(), res.size(), IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 100.f), false, lineWidth);
    if(res.size()==2)
        drawList->AddLine(res[0], res[1], IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), lineWidth);
    else if (res.size() == 3)
    {
        drawList->AddBezierCurve(res[0], res[0], res[1], res[2], IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), lineWidth);
    }
    else if(res.size() == 4)
    {
        drawList->AddBezierCurve(res[0], res[1], res[2], res[3], IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), lineWidth);
    }
}

Chaf::Primitive* Chaf::Bezier::Update(const glm::vec2& pos)
{
    if (start && pos == *pointList[0])
    {
        end = true;
        pointList.push_back(pointList[0]);
        return this;
    }
    if (!start)start = true;
    pointList.push_back(new glm::vec2(pos));
    if (pointList.size() == 4)
        end = true;
    return this;
}
