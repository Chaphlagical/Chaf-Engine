#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <Engine/core.h>
#include <Renderer/vao.h>
#include <Renderer/texture.h>
#include <Renderer/shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <vector>

namespace Chaf
{
	/*struct RenderData
	{
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Texture2D> m_DefaultTexture;
	};*/

	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Normal;
		bool operator == (const Vertex& v) const
		{
			return m_Position == v.m_Position &&
				m_TexCoord == v.m_TexCoord &&
				m_Normal == v.m_Normal;
		};
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

namespace std {
	template<> struct hash<Chaf::Vertex> {
		size_t operator()(Chaf::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.m_Position) ^
				(hash<glm::vec3>()(vertex.m_Normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.m_TexCoord) << 1);
		}
	};
}

#define MAX_TEXTURE_NUM 10