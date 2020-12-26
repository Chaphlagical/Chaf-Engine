#include <Canvas/Canvas.h>
#include <iostream>
void Chaf::Canvas::Draw(ImDrawList* drawList, const glm::vec2& origin)
{
    if (m_shapes.empty())return;
    for (auto shape : m_shapes)
    {
        shape->Draw(drawList, origin);
    }
}

bool Chaf::Canvas::IsFinish()
{
    if (m_shapes.empty())return true;
    return m_shapes.back()->end;
}

Chaf::Primitive* Chaf::Canvas::GetLast()
{
    if (m_shapes.empty())return nullptr;
    return m_shapes.back();
}

void Chaf::Canvas::AddShape(Primitive* primitive)
{
    if (!m_shapes.empty() && m_shapes.back() == primitive)
        return;
    m_shapes.push_back(primitive);
}

Chaf::Primitive* Chaf::Canvas::GetShape(glm::vec2* point)
{
    for (auto shape = m_shapes.begin(); shape != m_shapes.end(); shape++)
    {
        auto it = std::find((*shape)->pointList.begin(), (*shape)->pointList.end(), point);
        if (it != (*shape)->pointList.end())
            return *shape;
    }
}

void Chaf::Canvas::Erase(glm::vec2* point)
{
    for (auto shape = m_shapes.begin(); shape != m_shapes.end(); shape++)
    {
        auto it = std::find((*shape)->pointList.begin(), (*shape)->pointList.end(), point);
        if (it != (*shape)->pointList.end())
        {
            (*shape)->pointList.erase(it);
            m_shapes.erase(shape);
            return;
        }
    }
}

void Chaf::Canvas::Clear()
{
    while (!m_shapes.empty())
    {
        m_shapes.erase(m_shapes.begin());
    }
}

glm::vec2* Chaf::Canvas::Pick(const glm::vec2& pos)
{
    for (auto& shape : m_shapes)
    {
        for (size_t n = 0; n < shape->pointList.size(); n++)
        {
            auto point = shape->pointList[n];
            if (glm::distance(*point, pos) < shape->pointSize * 2)
                return point;
        }
    }
    return nullptr;
}
