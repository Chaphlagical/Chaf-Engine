#pragma once

#include <Renderer/shader.h>
#include <Renderer/mesh.h>
#include <Renderer/light.h>
#include <Renderer/material.h>
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

	/*struct MaterialComponent
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
	};*/

	struct MaterialComponent
	{
		Ref<Material> MaterialSrc = CreateRef<Material>();
		MaterialType Type = MaterialType::Material_None;
		MaterialComponent() = default;
		MaterialComponent(MaterialType type)
			:Type(type)
		{
			switch (type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			default:
				break;
			}
			CHAF_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void SetMaterialType(MaterialType type)
		{
			MaterialSrc.reset();
			Type = type;
			switch (type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			default:
				break;
			}
			CHAF_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void ResetType()
		{
			MaterialSrc.reset();
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc = CreateRef<Material>();
				return;
			case MaterialType::Material_Emission:
				MaterialSrc = CreateRef<EmissionMaterial>();
				return;
			case MaterialType::Material_Phong:
				MaterialSrc = CreateRef<PhongMaterial>();
				return;
			default:
				break;
			}
			CHAF_CORE_ASSERT(false, "Unavailable Material Type");
		}

		void Bind()
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc->Bind();
				return;
			case MaterialType::Material_Emission:
				CastRef<EmissionMaterial>(MaterialSrc)->Bind();
				return;
			case MaterialType::Material_Phong:
				CastRef<PhongMaterial>(MaterialSrc)->Bind();
				return;
			default:
				break;
			}
		}

		void Set(Camera& camera, glm::mat4& transform)
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				MaterialSrc->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
				MaterialSrc->m_Shader->SetMat4("u_Transform", transform);
				return;
			case MaterialType::Material_Emission:
			{
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
				CastRef<EmissionMaterial>(MaterialSrc)->m_Shader->SetMat4("u_Transform", transform);
				return;
			}
			case MaterialType::Material_Phong:
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
				CastRef<PhongMaterial>(MaterialSrc)->m_Shader->SetMat4("u_Transform", transform);
				return;
			default:
				break;
			}
		}

		Ref<Shader>& GetShader()
		{
			switch (Type)
			{
			case MaterialType::Material_None:
				return MaterialSrc->m_Shader;
			case MaterialType::Material_Emission:
				return CastRef<EmissionMaterial>(MaterialSrc)->m_Shader;
			case MaterialType::Material_Phong:
				return CastRef<PhongMaterial>(MaterialSrc)->m_Shader;
			default:
				break;
			}
		}
	};

	struct MeshComponent
	{
		Ref<TriMesh> Mesh = CreateRef<TriMesh>();
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<TriMesh>& mesh)
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
		Ref<Light> LightSrc = CreateRef<Light>();
		LightType Type = LightType::LightType_Basic;

		LightComponent() = default;

		void SetLight(LightType type)
		{
			LightSrc.reset();
			switch (type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CHAF_ASSERT(false, "Unavailable light type!");
		}

		void ResetType()
		{
			LightSrc.reset();
			switch (Type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CHAF_ASSERT(false, "Unavailable light type!");
		}

		void SetType(LightType type)
		{
			LightSrc.reset();
			Type = type;
			switch (type)
			{
			case LightType::LightType_None:
				LightSrc = nullptr;
				return;
			case LightType::LightType_Basic:
				LightSrc = CreateRef<Light>();
				return;
			case LightType::LightType_DirLight:
				LightSrc = CreateRef<DirLight>();
				return;
			case LightType::LightType_PointLight:
				LightSrc = CreateRef<PointLight>();
				return;
			case LightType::LightType_SpotLight:
				LightSrc = CreateRef<SpotLight>();
				return;
			default:
				break;
			}
			CHAF_ASSERT(false, "Unavailable light type!");
		}

		void Bind(const Ref<Shader>& shader, const glm::vec3& position, const Camera& camera, const uint32_t& slot = 0)
		{
			switch (Type)
			{
			case LightType::LightType_None:
				return;
			case LightType::LightType_Basic:
				LightSrc->Bind(shader, position, camera, slot);
				return;
			case LightType::LightType_DirLight:
				CastRef<DirLight>(LightSrc)->Bind(shader, position, camera, slot);
				return;
			case LightType::LightType_PointLight:
				CastRef<PointLight>(LightSrc)->Bind(shader, position, camera, slot);
				return;
			case LightType::LightType_SpotLight:
				CastRef<SpotLight>(LightSrc)->Bind(shader, position, camera, slot);
				return;
			default:
				break;
			}
			CHAF_ASSERT(false, "Unavailable light type!");
		}
	};
}