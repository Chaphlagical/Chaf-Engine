#pragma once

#include <Engine/layer.h>
#include <Scene/entity.h>
#include <Scene/scene.h>
#include <Renderer/mesh.h>
#include <Renderer/texture.h>
#include <unordered_map>
#include	<Canvas/Canvas.h>
#include <Canvas/EShapeType.h>


namespace Chaf
{
	struct DrawData
	{
		glm::vec2 scrolling{0.f, 0.f};
		bool active{ true };
		bool opt_enable_context_menu{ true };
		bool is_selected{ false };
		bool is_editing{ false };
		glm::vec2* selectPoint = nullptr;
	};

	class CanvasLayer : public Layer
	{
	public:
		CanvasLayer() {}
		~CanvasLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override {};
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override {};
	private:
		DrawData m_data;
		Canvas m_canvas;
		EShapeType m_currentType{ EShapeType::POINT };
	};

}