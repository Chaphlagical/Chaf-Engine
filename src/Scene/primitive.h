#pragma once

#include <Engine/core.h>
#include <Renderer/vao.h>
#include <Renderer/texture.h>
#include <Renderer/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Chaf
{
	struct RenderData
	{
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_DefaultTexture;
	};

	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Normal;
	};

	struct Posture
	{
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_Transform;
		void Update()
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
			glm::mat4 rotation = glm::mat4_cast(glm::qua<float>(glm::radians(m_Rotation)));
			m_Transform = glm::scale(translation * rotation, m_Scale);
		};
	};

	struct Triangle
	{
		uint32_t idx1;
		uint32_t idx2;
		uint32_t idx3;
	};

	enum class MeshType
	{
		None = 0, Plane = 1, Cube = 2, Sphere = 3, Model=4
	};
}