#include "CAGDLayer.h"
#include "polynomial.h"
#include "RBF.h"

#include <Engine/input_system.h>

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
		if (m_Data.need_update)
		{
			m_Data.need_update = false;

			m_Data.xs.resize(m_Data.pos.size());
			m_Data.ys.resize(m_Data.pos.size());

			m_Data.inter_x.resize(10000);

			for (uint32_t i = 0; i < 10000; i++)
			{
				m_Data.inter_x[i] = m_Data.width.x + (double)i * m_Data.range.x / 10000.f;
			}

			std::sort(m_Data.pos.begin(), m_Data.pos.end(), [](glm::vec2 lhs, glm::vec2 rhs) {return lhs.x < rhs.x; });

			for (uint32_t i = 0; i < m_Data.pos.size(); i++)
			{
				m_Data.xs[i] = m_Data.pos[i].x;
				m_Data.ys[i] = m_Data.pos[i].y;
			}

			if (m_Data.pos.size() > 1)
			{
				PolynomialInterpolation polynomial(m_Data.pos);
				m_Data.polynomial = polynomial.evaluate(m_Data.inter_x);
				RBFInterpolation RBF(m_Data.pos, static_cast<double>(m_RBFd));
				m_Data.RBF = RBF.evaluate(m_Data.inter_x);
			}

		}

		// Check around
		m_Data.has_around = false;

		for (uint32_t i = 0; i < m_Data.xs.size(); i++)
		{
			if ((m_Data.current_pos.x - m_Data.xs[i]) * (m_Data.current_pos.x - m_Data.xs[i]) / (m_Data.range.x * m_Data.range.x) + (m_Data.current_pos.y - m_Data.ys[i]) * (m_Data.current_pos.y - m_Data.ys[i]) / (m_Data.range.y * m_Data.range.y) < 0.0005f)
			{
				m_Data.has_around = true;
				m_Data.around_index = i;
				break;
			}
		}

	}

	void CAGDLayer::OnImGuiRender()
	{
		auto* window = Application::Get().GetWindow().GetNativeWindow();

		ImGui::Begin("Homework 1");
		auto* draw_list = ImGui::GetWindowDrawList();

		if (ImGui::Button("Clear"))
		{
			m_Data.pos.clear();
			m_Data.need_update = true;
			m_Data.has_around = false;
			m_Data.has_select = false;
		}

		ImGui::SameLine();
		ImGui::Checkbox("Polynomial", &m_EnablePolynomial);
		ImGui::SameLine();
		ImGui::Checkbox("RBF", &m_EnableRBF);
		ImGui::SameLine();
		float old_d = m_RBFd;
		ImGui::DragFloat("RBF parameter", &m_RBFd, 0.001f);
		if (std::fabs(old_d - m_RBFd) < 10e-5)
		{
			m_Data.need_update = true;
		}

		if (ImPlot::BeginPlot("Interpretation", "x", "y", ImVec2(-1, -1)))
		{
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5.f);
			ImPlot::PlotScatter("data", m_Data.xs.data(), m_Data.ys.data(), (int)m_Data.xs.size());
			if (m_Data.pos.size() > 1)
			{
				if (m_EnablePolynomial)
				{
					ImPlot::PlotLine("Polynomial", m_Data.inter_x.data(), m_Data.polynomial.data(), (int)m_Data.polynomial.size());
				}

				if (m_EnableRBF)
				{
					ImPlot::PlotLine("RBF", m_Data.inter_x.data(), m_Data.RBF.data(), (int)m_Data.RBF.size());
				}
			}

			m_Data.current_pos = { ImPlot::GetPlotMousePos().x ,ImPlot::GetPlotMousePos().y };

			auto selection = ImPlot::GetPlotLimits();
			m_Data.range.x = selection.X.Max - selection.X.Min;
			m_Data.range.y = selection.Y.Max - selection.Y.Min;
			m_Data.width.x = selection.X.Min;
			m_Data.width.y = selection.X.Max;

			if (std::fabs(m_Data.step - m_Data.range.x / 1000.f) > 10e-5)
			{
				m_Data.step = m_Data.range.x / 1000.f;
				m_Data.need_update = true;
			}

			if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle) && ImGui::IsWindowFocused())
			{
				if (m_Data.has_around)
				{
					m_Data.select_index = m_Data.around_index;
					m_Data.pos[m_Data.select_index] = m_Data.current_pos;
					m_Data.xs[m_Data.select_index] = m_Data.current_pos.x;
					m_Data.ys[m_Data.select_index] = m_Data.current_pos.y;
					m_Data.has_select = true;
					m_Data.need_update = true;
				}
			}

			if (m_Data.has_select)
			{
				if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Delete)))
				{
					m_Data.has_select = false;
					m_Data.pos.erase(m_Data.pos.begin() + m_Data.select_index);
					m_Data.need_update = true;
				}
			}

			if (!m_Data.has_around && ImGui::IsMouseClicked(ImGuiMouseButton_Middle) && ImGui::IsWindowFocused() && !ImGui::IsMouseDragging(ImGuiMouseButton_Middle))
			{
				m_Data.pos.push_back(m_Data.current_pos);
				m_Data.need_update = true;
			}

			m_Data.has_select = false;

			ImPlot::EndPlot();
		}

		ImGui::End();
	}

	void CAGDLayer::OnEvent(Event& event)
	{

	}
}