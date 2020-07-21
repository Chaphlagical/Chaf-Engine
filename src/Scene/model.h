#pragma once

#include <Engine/core.h>
#include <Renderer/camera.h>
#include <Scene/primitive.h>
#include <vector>
#include <glm/glm.hpp>
#include <string>

namespace Chaf
{
	class CHAF_API TriMesh
	{
	public:
		TriMesh() {}
		~TriMesh() = default;

		void Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		void Create(MeshType type, int sample = 1);
		
		void SetTexture(const std::string path);
		void ResetTexture();
		void SetShader(const std::string path);

		void Draw(const Camera& camera, const bool lineMode = false);
	public:
		void SetPosition(const glm::vec3 position) { m_Posture.m_Position = position; m_Posture.Update(); }
		void SetPosition(const float x, const float y, const float z) { m_Posture.m_Position = { x,y,z }; m_Posture.Update(); }
		void SetRotation(const glm::vec3 rotation) { m_Posture.m_Rotation = rotation; m_Posture.Update(); }
		void SetRotation(const float x, const float y, const float z) { m_Posture.m_Rotation = { x,y,z }; m_Posture.Update(); }
		void SetScale(const glm::vec3 scale) { m_Posture.m_Scale = scale; m_Posture.Update(); }
		void SetScale(const float x, const float y, const float z) { m_Posture.m_Scale = { x,y,z }; m_Posture.Update(); }
		glm::vec3& GetPosition() { return m_Posture.m_Position; }
		glm::vec3& GetRotation() { return m_Posture.m_Rotation; }
		glm::vec3& GetScale() { return m_Posture.m_Scale; }
		glm::mat4& GetTransform() { return m_Posture.m_Transform; }

	private:
		void InitMesh();
	private:
		void CreatePlane(int sample);
	private:
		MeshType m_Type;
		Scope<RenderData> m_RenderData;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Triangle> m_Triangle;
		glm::vec4 m_Color = glm::vec4(1.0f);
		Posture m_Posture = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::mat4(1.0f) };
	};
}