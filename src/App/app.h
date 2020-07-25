#pragma once

#include <Engine/layer.h>
#include <Engine/time.h>
#include <Renderer/buffer.h>
#include <Renderer/texture.h>
#include <Renderer/vao.h>
#include <Scene/camera_controller.h>
#include <Scene/mesh.h>

namespace Chaf
{
	class AppLayer : public Layer
	{
	public:
		AppLayer();
		virtual ~AppLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	};
}