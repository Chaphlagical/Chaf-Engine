#include <Canvas/Oval.h>
#include <imgui.h>

#define PI 3.1415926535

void Chaf::Oval::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    DrawPoint(drawList, origin);
    if (pointList.size() == 2)
    {
        auto p0 = *pointList[0];
        auto p1 = *pointList[1];

        float a = fabs(p0.x - p1.x) / 2.f;
        float b = fabs(p0.y - p1.y) / 2.f;
        glm::vec2 center = glm::vec2{ p0.x + p1.x, p0.y + p1.y } / 2.f;
        std::vector<ImVec2> res;
        float angle = 0.f;
        while (angle < 2 * PI)
        {
            res.push_back({ center.x + a * cosf(angle) + origin.x, center.y + b * sinf(angle) +origin.y });
            angle += 0.01f;
        }
        drawList->AddConvexPolyFilled(res.data(), res.size(), IM_COL32(fillColor.r * 255.f, fillColor.g * 255.f, fillColor.b * 255.f, fillColor.a * 255.f));
        drawList->AddPolyline(res.data(), res.size(), IM_COL32(lineColor.r * 255.f, lineColor.g * 255.f, lineColor.b * 255.f, lineColor.a * 255.f), true, lineWidth);
    }

}

Chaf::Primitive* Chaf::Oval::Update(const glm::vec2& pos)
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
