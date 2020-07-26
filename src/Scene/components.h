#pragma once

#include <Renderer/shader.h>
#include <Renderer/mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

namespace Chaf
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f };
		glm::vec3 Rotation{ 0.0f };
		glm::vec3 Scale{ 1.0f };

		glm::mat4 Transform{ 1.0f };
		glm::vec3 RelatePosition{ 0.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			:Position(position), Rotation(rotation), Scale(scale)
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position + RelatePosition);
			glm::mat4 rotation_ = glm::mat4_cast(glm::qua<float>(glm::radians(Rotation)));
			Transform = glm::scale(translation * rotation_, Scale);
		}

		void Update()
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position + RelatePosition);
			glm::mat4 rotation = glm::mat4_cast(glm::qua<float>(glm::radians(Rotation)));
			Transform = glm::scale(translation * rotation, Scale);
		}

		void SetRelatePosition(const glm::vec3& position) 
		{
			Position = Position + RelatePosition;
			RelatePosition = position; 
			Position = Position - position; 
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MaterialComponent
	{
		Ref<Texture2D> AlbedoTexture = nullptr;
		glm::vec4 Color{ 1.0f };
		bool HasAlbedo = false;
		//	TODO: more material
		MaterialComponent()
		{
			AlbedoTexture = Texture2D::Create(1, 1);
			uint32_t defaultTextureData = 0xffffffff;
			AlbedoTexture->SetData(&defaultTextureData, sizeof(uint32_t));
			Color = glm::vec4{ 1.0f };
		}

		MaterialComponent(const MaterialComponent&) = default;
		MaterialComponent(const Ref<Texture2D>& albedoTexture, glm::vec4& color)
			: AlbedoTexture(albedoTexture), Color(color), HasAlbedo(true){}
		MaterialComponent(const std::string& path)
			: AlbedoTexture(Texture2D::Create(path)), Color({ 1.0f }), HasAlbedo(true) {}
		MaterialComponent(const glm::vec4& color)
			: Color(color), HasAlbedo(false)
		{
			AlbedoTexture = Texture2D::Create(1, 1);
			uint32_t defaultTextureData = 0xffffffff;
			AlbedoTexture->SetData(&defaultTextureData, sizeof(uint32_t));
		}

		MaterialComponent(const std::string& path, const glm::vec4& color)
			: AlbedoTexture(Texture2D::Create(path)), Color(color), HasAlbedo(true){}

		//	Use
		void Bind(const Ref<Shader>& shader)
		{
			shader->Bind();
			AlbedoTexture->Bind(0);
			shader->SetFloat4("u_Color", Color);
		}

		//	Setting
		void SetTexture(const std::string& path)
		{
			HasAlbedo = true;
			AlbedoTexture.reset();
			AlbedoTexture = Texture2D::Create(path);
		}

		void ResetTexture()
		{
			HasAlbedo = false;
			AlbedoTexture.reset();
			AlbedoTexture = Texture2D::Create(1, 1);
			uint32_t defaultTextureData = 0xffffffff;
			AlbedoTexture->SetData(&defaultTextureData, sizeof(uint32_t));
		}

		void SetColor(const glm::vec4& color) { Color = color; }
	};

	struct MeshComponent
	{
		Ref<TriMesh> Mesh = CreateRef<TriMesh>();
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<TriMesh> & mesh)
			: Mesh(mesh) {}
		MeshComponent(const std::string& path)
			:Mesh(TriMesh::Create(path)) {}
		MeshComponent(const MeshType& type, const uint32_t& sample = 1)
			:Mesh(TriMesh::Create(type, sample)) {}

		void Reset() { Mesh.reset(); Mesh = TriMesh::Create(); }
		void Reload(const std::string& path)
		{
			Mesh.reset();
			Mesh = TriMesh::Create(path);
		}
		void Reload(const MeshType& type, const uint32_t& sample = 1)
		{
			Mesh.reset();
			Mesh = TriMesh::Create(type, sample);
		}
	};

	struct LightComponent
	{
		float AmbientStrength = 0;
		glm::vec4 LightColor = glm::vec4{ 1.0f };

	};
}