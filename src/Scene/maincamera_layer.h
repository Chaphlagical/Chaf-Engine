#pragma once

#include <Engine/core.h>
#include <Engine/layer.h>
#include <Scene/camera_controller.h>

namespace Chaf
{
	class CHAF_API MainCameraLayer :public Layer
	{
	public:
		MainCameraLayer();
		virtual ~MainCameraLayer() = default;

		virtual void OnAttach() override {};
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override {};
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override {};

	public:
		static MainCameraLayer* GetInstance() { return s_Instance; }
		CameraController& GetCameraController() { return m_CameraController; }

	private:
		CameraController m_CameraController;
		static MainCameraLayer* s_Instance;
	};
}