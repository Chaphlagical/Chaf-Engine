#include <Engine/log.h>
#include <Engine/core.h>

#include <Renderer/camera.h>

#include <glm/gtc/quaternion.hpp>
#include <iostream>
namespace Chaf
{
	Camera::Camera(float fov, float aspect, CameraType type, float nearPlane, float farPlane)
		:m_Fov(fov), m_Aspect(aspect), m_Type(type), m_NearPlane(nearPlane), m_FarPlane(farPlane), m_ViewMatrix(1.0f)
	{
		UpdateProjectionMatrix();
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 front = glm::vec3(1.0f);

		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		m_Front = glm::normalize(front);

		// right vector
		m_Right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

		// up vector
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
		UpdateViewProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix()
	{
		switch (m_Type)
		{
		case Chaf::CameraType::None:
			CHAF_ASSERT(false, "Unknown CameraType!");
			return;
		case Chaf::CameraType::Orthographic:
			m_ProjectionMatrix = glm::ortho(-glm::radians(m_Fov), glm::radians(m_Fov), -glm::radians(m_Fov) / m_Aspect, glm::radians(m_Fov) / m_Aspect, m_NearPlane, m_FarPlane);
			UpdateViewProjectionMatrix();
			return;
		case Chaf::CameraType::Perspective:
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, m_NearPlane, m_FarPlane);
			UpdateViewProjectionMatrix();
			return;
		}
		CHAF_ASSERT(false, "Unknown CameraType!");
		return;
	}

}