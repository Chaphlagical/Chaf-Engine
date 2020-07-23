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
		TriMesh(std::string name="mesh"):m_Type(MeshType::None), m_Name(name) {}
		~TriMesh() = default;

		void Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		void Create(MeshType type, int sample = 1);
		void Create(const std::string path);
		
		void SetTexture(const std::string path, const uint32_t index);
		void ResetTexture(const uint32_t index);
		void AddTexture(const std::string path);
		void AddTexture();
		void DeleteTexture(const uint32_t index);
		const uint32_t GetTextureNum() { return m_RenderData->m_Texture.size(); }
		float GetTextureWeight(const uint32_t index) { return m_TextureWeights[index]; }

		void SetShader(const std::string path);

		void Draw(const Camera& camera, const bool lineMode = false) const;
	public:
		void SetPosition(const glm::vec3 position) { m_Posture.m_Position = position; m_Posture.Update(); }
		void SetPosition(const float x, const float y, const float z) { m_Posture.m_Position = { x,y,z }; m_Posture.Update(); }
		void SetRotation(const glm::vec3 rotation) { m_Posture.m_Rotation = rotation; m_Posture.Update(); }
		void SetRotation(const float x, const float y, const float z) { m_Posture.m_Rotation = { x,y,z }; m_Posture.Update(); }
		void SetScale(const glm::vec3 scale) { m_Posture.m_Scale = scale; m_Posture.Update(); }
		void SetScale(const float x, const float y, const float z) { m_Posture.m_Scale = { x,y,z }; m_Posture.Update(); }
		void SetLineMode(const bool enable) { m_LineMode = enable; }
		void SetName(const std::string name) { m_Name = name; }
		void SetColor(const glm::vec4 color) { m_Color = color; }
		void SetTextureWeight(const uint32_t index, const float val) { m_TextureWeights[index] = val; }

		glm::vec3& GetPosition() { return m_Posture.m_Position; }
		glm::vec3& GetRotation() { return m_Posture.m_Rotation; }
		glm::vec3& GetScale() { return m_Posture.m_Scale; }
		glm::mat4& GetTransform() { return m_Posture.m_Transform; }
		std::string& GetName() { return m_Name; }
		glm::vec4& GetColor() { return m_Color; }
		Ref<Texture2D>& GetTexture(const uint32_t index) { return m_RenderData->m_Texture[index]; }
		bool& GetLineMode() { return m_LineMode; }
		const bool HasTexture(const uint32_t index) { return m_HasTexture[index]; }

	private:
		void InitMesh();
	private:
		void CreatePlane(int sample);
		void CreateCube(int sample);
		void CreateSphere(int sample);
		void ObjLoader(const std::string path);
	private:
		std::string m_Name;
		MeshType m_Type;
		bool m_LineMode = false;
		std::vector<bool> m_HasTexture;
		std::vector<float> m_TextureWeights;
		Scope<RenderData> m_RenderData;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<Triangle> m_Triangle;
		glm::vec4 m_Color = glm::vec4(1.0f);
		Posture m_Posture = { glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::mat4(1.0f) };
	};
}