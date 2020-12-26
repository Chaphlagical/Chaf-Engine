#include <Canvas/CanvasLayer.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <Canvas/Point.h>
#include <Canvas/Rectangle.h>
#include <Canvas/Oval.h>
#include <Canvas/Circle.h>
#include <Canvas/Line.h>
#include <Canvas/Polygon.h>
#include <Canvas/Bezier.h>

#include <glm/gtc/type_ptr.hpp>

void Chaf::CanvasLayer::OnAttach()
{

}
#include <iostream>
void Chaf::CanvasLayer::OnImGuiRender()
{
	ImGui::Begin("Canvas");

	Primitive* shape = m_canvas.GetLast();

	auto previousType = m_currentType;
	bool drawType = m_currentType == EShapeType::NONE ? true : false;
	ImGui::Checkbox("None", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::NONE : m_currentType;

	drawType = m_currentType == EShapeType::POINT ? true : false;
	ImGui::Checkbox("Point", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::POINT : m_currentType;

	drawType = m_currentType == EShapeType::RECTANGLE ? true : false;
	ImGui::Checkbox("Rectangle", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::RECTANGLE : m_currentType;

	drawType = m_currentType == EShapeType::OVAL ? true : false;
	ImGui::Checkbox("Oval", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::OVAL : m_currentType;

	drawType = m_currentType == EShapeType::CIRCLE ? true : false;
	ImGui::Checkbox("Circle", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::CIRCLE : m_currentType;

	drawType = m_currentType == EShapeType::LINE ? true : false;
	ImGui::Checkbox("Line", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::LINE : m_currentType;

	drawType = m_currentType == EShapeType::POLYGON ? true : false;
	ImGui::Checkbox("Polygon", &drawType);
	ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::POLYGON : m_currentType;

	drawType = m_currentType == EShapeType::BEZIER ? true : false;
	ImGui::Checkbox("Cubic Spline", &drawType);
	//ImGui::SameLine();
	m_currentType = m_canvas.IsFinish() && drawType ? EShapeType::BEZIER : m_currentType;

	switch (m_currentType)
	{
	case Chaf::EShapeType::NONE:
		shape = nullptr;
		break;
	case Chaf::EShapeType::POINT:
		shape = m_canvas.IsFinish() ? new Point : static_cast<Point*>(shape);
		break;
	case Chaf::EShapeType::LINE:
		shape = m_canvas.IsFinish() ? new Line : static_cast<Line*>(shape);
		break;
	case Chaf::EShapeType::CIRCLE:
		shape = m_canvas.IsFinish() ? new Circle : static_cast<Circle*>(shape);
		break;
	case Chaf::EShapeType::RECTANGLE:
		shape = m_canvas.IsFinish() ? new Rectangle : static_cast<Rectangle*>(shape);
		break;
	case Chaf::EShapeType::OVAL:
		shape = m_canvas.IsFinish() ? new Oval : static_cast<Oval*>(shape);
		break;
	case Chaf::EShapeType::BEZIER:
		shape = m_canvas.IsFinish() ? new  Bezier: static_cast<Bezier*>(shape);
		break;
	case Chaf::EShapeType::POLYGON:
		shape = m_canvas.IsFinish() ? new Polygon : static_cast<Polygon*>(shape);
		break;
	default:
		break;
	}

	static bool colorEdit = false;
	ImGui::SameLine();
	if (ImGui::Button("Edit Color"))
		colorEdit = !colorEdit && m_data.selectPoint;

	if (colorEdit && m_data.selectPoint)
	{
		ImGui::ColorEdit4("Point Color", (float*)(&m_canvas.GetShape(m_data.selectPoint)->pointColor));
		ImGui::ColorEdit4("Line Color", (float*)(&m_canvas.GetShape(m_data.selectPoint)->lineColor));
		ImGui::ColorEdit4("Fill Color", (float*)(&m_canvas.GetShape(m_data.selectPoint)->fillColor));
		ImGui::DragFloat("Point Size", &m_canvas.GetShape(m_data.selectPoint)->pointSize, 0.1f, 0.01f, 30.f);
		ImGui::DragFloat("Line Width", &m_canvas.GetShape(m_data.selectPoint)->lineWidth, 0.1f, 0.01f, 10.f);
	}


	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
	ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
	ImGuiIO& io = ImGui::GetIO();
	if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
	if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
	ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
	const ImVec2 origin(canvas_p0.x + m_data.scrolling[0], canvas_p0.y + m_data.scrolling[1]); // Lock scrolled origin
	const glm::vec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);
	const float mouse_threshold_for_pan = m_data.opt_enable_context_menu ? -1.0f : 0.0f;

	// Draw border and background color
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
	draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
	ImGui::InvisibleButton("canvas", canvas_sz);

	// mouse dragging moving
	if (m_data.active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
	{
		m_data.scrolling[0] += io.MouseDelta.x;
		m_data.scrolling[1] += io.MouseDelta.y;
		m_data.is_editing = false;
		m_data.is_selected = false;
		m_data.selectPoint = nullptr;
	}

	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held

	// point select
	if (is_hovered && !ImGui::IsMouseDragging(ImGuiMouseButton_Left, mouse_threshold_for_pan) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		m_data.selectPoint = m_canvas.Pick(mouse_pos_in_canvas);
		m_data.is_selected = !(m_data.selectPoint == nullptr);
	}
	

	// point editing
	if (is_active && m_data.is_selected && ImGui::IsMouseDragging(ImGuiMouseButton_Left, mouse_threshold_for_pan))
	{
		*m_data.selectPoint = { mouse_pos_in_canvas.x, mouse_pos_in_canvas.y };
	}

	if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (shape&&!m_data.is_selected && !m_data.is_editing)
		{
			shape->Update(mouse_pos_in_canvas);
		}
	}

	// add shape to canvas
	if (shape&&shape->start)
	{
		m_canvas.AddShape(shape);
		m_data.selectPoint = shape->pointList.back();
		m_data.is_selected = true;
	}
		
	//context menu
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
		ImGui::OpenPopupContextItem("context");
	if (ImGui::BeginPopup("context"))
	{
		if (ImGui::MenuItem("Finish", NULL, false, shape&&!shape->pointList.empty() && !shape->end))
		{
			while (shape && !shape->end)
				shape->Update(*(shape->pointList[0]));
		}
		if (ImGui::MenuItem("Delete", NULL, false, !(m_data.selectPoint == nullptr)))
		{
			m_canvas.Erase(m_data.selectPoint);
			m_data.is_editing = false;
			m_data.is_selected = false;
			m_data.selectPoint = nullptr;
		}
		if (ImGui::MenuItem("Clear", NULL, false))
		{
			m_canvas.Clear();
			m_data.is_editing = false;
			m_data.is_selected = false;
			m_data.selectPoint = nullptr;
		}
		ImGui::EndPopup();
	}

	// Draw Grid
	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (true)
	{
		const float GRID_STEP = 64.0f;
		for (float x = fmodf(m_data.scrolling[0], GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
		for (float y = fmodf(m_data.scrolling[1], GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
			draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
	}

	m_canvas.Draw(draw_list, glm::vec2(origin.x, origin.y));
	if (m_data.is_selected)
		draw_list->AddCircle(ImVec2(origin.x + (*m_data.selectPoint)[0], origin.y + (*m_data.selectPoint)[1]), m_canvas.GetShape(m_data.selectPoint)->pointSize*2, IM_COL32(255, 255, 0, 255), m_canvas.GetShape(m_data.selectPoint)->pointSize * 4);
	ImGui::End();
}
