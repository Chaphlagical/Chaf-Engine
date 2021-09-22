#include "CAGDLayer.h"
#include "BezierCurve.h"

#include <implot.h>

namespace Chaf
{
	CAGDLayer::CAGDLayer()
	{

	}

	void CAGDLayer::OnAttach()
	{
		ImPlot::CreateContext();
	}

	void CAGDLayer::OnDetach()
	{
		ImPlot::DestroyContext();
	}

	void CAGDLayer::OnUpdate(Timestep timestep)
	{
		if (m_Updated && m_Data.control_x.size() > 1)
		{
			m_Updated = false;
			BezierCurve::create(m_Data.control_x, m_Data.control_y, m_Data.curve_x, m_Data.curve_y);
		}
	}

	void CAGDLayer::OnImGuiRender()
	{
		ImGui::Begin("Homework 2");

		if (ImGui::Button("Clear"))
		{
			m_Data.control_x.clear();
			m_Data.control_y.clear();
			m_Data.curve_x.clear();
			m_Data.curve_y.clear();
			m_Updated = true;
		}

		if (ImPlot::BeginPlot("Bezier Curve", "x", "y", ImVec2(-1, -1)))
		{
			uint32_t index = 0;
			ImPlotStyle& style = ImPlot::GetStyle();

			for (auto i = 0; i < m_Data.control_x.size(); i++)
			{
				if (ImPlot::DragPoint(("P" + std::to_string(index++)).c_str(), &m_Data.control_x[i], &m_Data.control_y[i], true, ImVec4(0.f, 0.f, 0.f, -1.f), 8.f))
				{
					m_Updated = true;
				}
			}

			style.LineWeight = 1.f;
			ImPlot::PlotLine("Line", m_Data.control_x.data(), m_Data.control_y.data(), (int)m_Data.control_x.size());
			style.LineWeight = 4.f;
			ImPlot::PlotLine("Bezier Curve", m_Data.curve_x.data(), m_Data.curve_y.data(), (int)m_Data.curve_x.size());

			if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl)
			{
				ImPlotPoint pt = ImPlot::GetPlotMousePos();
				m_Data.control_x.push_back(pt.x);
				m_Data.control_y.push_back(pt.y);
				m_Updated = true;
			}

			ImPlot::EndPlot();
		}

		ImGui::End();
	}

	void CAGDLayer::OnEvent(Event& event)
	{

	}
}