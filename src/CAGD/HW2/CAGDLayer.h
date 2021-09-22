#pragma once

#include <Editor/application.h>

#include <glm/glm.hpp>

struct ImPlotPoint;

namespace Chaf
{
	struct DrawData
	{
		std::vector<double> control_x;
		std::vector<double> control_y;
		std::vector<double> curve_x;
		std::vector<double> curve_y;
	};

	class CAGDLayer : public Layer
	{
	public:
		CAGDLayer();
		virtual ~CAGDLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		DrawData m_Data;
		bool m_Updated = false;
	};
}