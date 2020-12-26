#pragma once

#include <Engine/time.h>
#include <Engine/event_system.h>

#include <Renderer/camera.h>

namespace Chaf
{
	class CHAF_API CameraController
	{
	public:
		CameraController(float fov, float aspect, CameraType type = CameraType::Perspective, float nearPlane = 0.1f, float farPlane = 100.0f, bool is_2D = false);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		void SetActive(const bool enable) { m_Enable = enable; }
		void SetSensitivity(const float sensitivity) { m_Sensitivity = sensitivity; }
		void SetSpeed(const float speed) { m_Speed = speed; }

		float& GetSensitivity() { return m_Sensitivity; }
		float& GetSpeed() { return m_Speed; }

		void Reset(float x, float y) { m_Last_X = x; m_Last_Y = 0; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		bool IsCameraControllerActive();
		bool IsMouseMiddlePress();

	private:
		Camera m_Camera;
		float m_Speed = 10.0f;
		float m_Last_X = 0, m_Last_Y = 0;
		float m_Sensitivity = 0.1f;
		float m_Enable = false;
	};
}