#pragma once

#include <Engine/core.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chaf
{
	enum class CHAF_API CameraType
	{
		None = 0,
		Orthographic = 1,
		Perspective = 2
	};

	class CHAF_API Camera
	{
	public:
		Camera(float fov, float aspect, CameraType type = CameraType::Perspective, float nearPlane = 0.1f, float farPlane = 100.0f);

		//	Get parameter
		const CameraType& GetCameraType() const { return m_Type; }
		
		const float& GetPitch() const { return m_Pitch; }
		const float& GetYaw() const { return m_Yaw; }
		const float& GetFov() const { return m_Fov; }
		const float& GetAspect() const { return m_Aspect; }
		
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetUpVector() const { return m_Up; }
		const glm::vec3& GetFrontVector() const { return m_Front; }
		const glm::vec3& GetRightVector() const { return m_Right; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		const float& GetNearPlane() const { return m_NearPlane; }
		const float& GetFarPlane() const { return m_FarPlane; }

		void SetPosition(const glm::vec3& position) 
		{ 
			if (m_Position == position)return;
			m_Position = position; UpdateViewMatrix(); 
		}

		void SetPitch(const float pitch) 
		{ 
			if (m_Pitch == pitch)return;
			m_Pitch = pitch; UpdateViewMatrix(); 
		}

		void SetYaw(const float yaw) 
		{ 
			if (m_Yaw == yaw)return;
			m_Yaw = yaw; UpdateViewMatrix(); 
		}

		void SetFov(float fov) 
		{
			if (m_Fov == fov)return;
			m_Fov = fov; UpdateProjectionMatrix(); 
		}

		void SetAspect(float aspect) 
		{
			if (m_Aspect == aspect)return;
			m_Aspect = aspect; UpdateProjectionMatrix(); 
		}

		void SetCameraType(const CameraType& type)
		{
			if (m_Type == type)return;
			m_Type = type;
			m_Pitch = 0.0f;
			m_Yaw = -90.0f;
			UpdateProjectionMatrix();
			UpdateViewMatrix();
		}

		void SetNearPlane(const float& val)
		{
			if (m_NearPlane == val)return;
			m_NearPlane = val;
			UpdateProjectionMatrix();
			UpdateViewMatrix();
		}

		void SetFarPlane(const float& val)
		{
			if (m_FarPlane == val)return;
			m_FarPlane = val;
			UpdateProjectionMatrix();
			UpdateViewMatrix();
		}

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateViewProjectionMatrix() { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }

	private:
		CameraType m_Type;
		float m_Fov, m_Aspect, m_FarPlane, m_NearPlane;
		float m_Pitch= -30.0f, m_Yaw = -60.0f;
		glm::vec3 m_Position = { -17.0f, 23.0f, 32.0f };
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f }, m_Up = { 0.0f, 1.0f, 0.0f }, m_Right = { 1.0f, 0.0f, 0.0f };
	};

}