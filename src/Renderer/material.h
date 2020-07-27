#pragma once

#include <Engine/core.h>
#include <Renderer/shader.h>
#include <Renderer/camera.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Chaf
{
	enum class MaterialType
	{
		Material_None = 0,
		Material_Emission = 1,
		Material_Phong = 2
	};

	static std::unordered_map < MaterialType, std::string > MaterialTypeMap =
	{
		{MaterialType::Material_None, "None"},
		{MaterialType::Material_Emission, "Emisson"},
		{MaterialType::Material_Phong, "Phong"}
	};

	struct Material
	{
		Ref<Shader> m_Shader = Shader::Create("assets/shader/material/default.glsl");
		Material() = default;
		virtual ~Material() {}
		virtual void Bind() { m_Shader->Bind(); };
	};

	struct EmissionMaterial :public Material
	{
		Ref<Shader> m_Shader = Shader::Create("assets/shader/material/emission.glsl");
		glm::vec3 EmissionColor{ 1.0f };
		float Intensity = 1.0f;
		EmissionMaterial() = default;
		virtual ~EmissionMaterial() {}
		virtual void Bind() override
		{
			m_Shader->Bind();
			m_Shader->SetFloat3("u_EmissionColor", EmissionColor);
			m_Shader->SetFloat("u_Intensity", Intensity);
		}
	};

	struct PhongMaterial :public Material
	{
		Ref<Shader> m_Shader = Shader::Create("assets/shader/material/phong.glsl");
		Ref<Texture2D> DiffuseTexture = nullptr;
		Ref<Texture2D> SpecularTexture = nullptr;
		glm::vec3 Color{ 1.0f };
		float Shininess = 32;
		PhongMaterial()
		{
			DiffuseTexture = Texture2D::Create(1, 1);
			SpecularTexture = Texture2D::Create(1, 1);
			uint32_t data = 0xffffffff;
			DiffuseTexture->SetData(&data, sizeof(uint32_t));
			SpecularTexture->SetData(&data, sizeof(uint32_t));
		}
		virtual ~PhongMaterial() {}
		virtual void Bind() override
		{
			m_Shader->Bind();
			DiffuseTexture->Bind(0);
			m_Shader->SetInt("u_Material.diffuse", 0);
			SpecularTexture->Bind(1);
			m_Shader->SetInt("u_Material.specular", 1);
			m_Shader->SetFloat("u_Material.shininess", Shininess);
		}

		void SetDiffuseTexture(const std::string& path)
		{
			DiffuseTexture.reset();
			DiffuseTexture = Texture2D::Create(path);
		}

		void SetSpecularTexture(const std::string& path)
		{
			SpecularTexture.reset();
			SpecularTexture = Texture2D::Create(path);
		}
	};
}