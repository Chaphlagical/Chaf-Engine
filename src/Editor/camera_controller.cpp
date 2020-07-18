#include <Editor/camera_controller.h>
#include <Editor/application.h>
#include <Engine/input_system.h>
#include <iostream>

namespace Chaf
{
	CameraController::CameraController(float fov, float aspect, CameraType type, float nearPlane, float farPlane, bool is_2D)
		:m_Camera(fov, aspect, type, nearPlane, farPlane)
	{

	}

	bool CameraController::IsCameraControllerActive()
	{
		auto window = Application::Get().GetWindow().GetNativeWindow();
		return Input::IsMouseButtonPressed(window, CHAF_MOUSE_BUTTON_RIGHT) && m_Enable;
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		glm::vec3 position = m_Camera.GetPosition();
		auto window = Application::Get().GetWindow().GetNativeWindow();

		if (IsCameraControllerActive())
		{	
			Input::SetCursorHidden(window, true);

			if (Input::IsKeyPressed(window, CHAF_KEY_A))
				position -= m_CameraTranslationSpeed * ts * m_Camera.GetRightVector();
			if (Input::IsKeyPressed(window, CHAF_KEY_D))
				position += m_CameraTranslationSpeed * ts * m_Camera.GetRightVector();
			if (Input::IsKeyPressed(window, CHAF_KEY_W))
				position += m_CameraTranslationSpeed * ts * m_Camera.GetFrontVector();
			if (Input::IsKeyPressed(window, CHAF_KEY_S))
				position -= m_CameraTranslationSpeed * ts * m_Camera.GetFrontVector();
			if (Input::IsKeyPressed(window, CHAF_KEY_Q))
				position += m_CameraTranslationSpeed * ts * m_Camera.GetUpVector();
			if (Input::IsKeyPressed(window, CHAF_KEY_E))
				position -= m_CameraTranslationSpeed * ts * m_Camera.GetUpVector();
		}
		else
			Input::SetCursorHidden(window,false);
		m_Camera.SetPosition(position);
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CHAF_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<MouseMovedEvent>(CHAF_BIND_EVENT_FN(CameraController::OnMouseMoved));
		dispatcher.Dispatch<WindowResizeEvent>(CHAF_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		float aspect = width / height;
		m_Camera.SetAspect(aspect);
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		

		if (IsCameraControllerActive())
		{
			float m_Zoom = m_Camera.GetFov();
			m_Zoom -= e.GetYOffset();
			if (m_Zoom <= 1.0f)
				m_Zoom = 1.0f;
			if (m_Zoom >= 45.0f)
				m_Zoom = 45.0f;
			m_Camera.SetFov(m_Zoom);
		}
		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		

		if (IsCameraControllerActive())
		{
			if (m_Camera.GetCameraType() == CameraType::Perspective)
			{
				float yaw = m_Camera.GetYaw(), pitch = m_Camera.GetPitch();
				yaw += (e.GetX() - m_Last_X) * m_Sensitivity;
				pitch -= (e.GetY() - m_Last_Y) * m_Sensitivity;

				if (pitch > 89.0f)
					pitch = 89.0f;
				else if (pitch < -89.0f)
					pitch = -89.0f;

				m_Camera.SetPitch(pitch);
				m_Camera.SetYaw(yaw);
			}
		}
		m_Last_X = e.GetX();
		m_Last_Y = e.GetY();

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		

		if (e.GetWidth() != 0 && e.GetHeight() != 0)
		{
			float aspect = (float)e.GetWidth() / (float)e.GetHeight();
			m_Camera.SetAspect(aspect);
		}
		return false;
	}
}
