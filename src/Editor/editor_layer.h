#pragma once

#include <Engine/layer.h>
#include <Scene/entity.h>
#include <Scene/scene.h>
#include <Renderer/mesh.h>
#include <Renderer/texture.h>
#include <unordered_map>

namespace Chaf
{
	class EditorLayer: public Layer
	{
	public:
		EditorLayer() {}
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override {};
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override {};
	};

}