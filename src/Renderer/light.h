#pragma once

#include <Renderer/camera.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace Chaf
{
	enum class LightType
	{
		LightType_None = 0,
		LightType_Basic = 1,
		LightType_DirLight = 2,
		LightType_PointLight = 3,
		LightType_SpotLight = 4
	};

	static std::unordered_map<LightType, std::string> LightTypeMap =
	{
		{LightType::LightType_None, "None"},
		{LightType::LightType_Basic, "Basic"},
		{LightType::LightType_DirLight, "DIrLight"},
		{LightType::LightType_PointLight, "PointLight"},
		{LightType::LightType_SpotLight, "SpotLight"}
	};

	struct Light
	{
		glm::vec3 Color{ 1.0f };

		Light() = default;

		virtual void Bind(const Ref<Shader>& shader, const glm::vec3& position, const Camera& camera, const uint32_t& slot)
		{
			shader->Bind();
			shader->SetFloat3("u_Light[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_Light[" + std::to_string(slot) + "].position", position);
			shader->SetFloat3("u_ViewPos", camera.GetPosition());
		}
	};

	struct DirLight : public Light
	{
		glm::vec3 Direction{ 1.0f };
		
		DirLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const glm::vec3& position, const Camera& camera, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].direction", Direction);
			shader->SetFloat3("u_ViewPos", camera.GetPosition());
		}
	};

	struct PointLight : public Light
	{
		float Constant = 0.0f;
		float Linear = 0.0f;
		float Quadratic = 0.0f;

		PointLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const glm::vec3& position, const Camera& camera, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", position);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
			shader->SetFloat3("u_ViewPos", camera.GetPosition());
		}
	};

	struct SpotLight : public Light
	{
		glm::vec3  Direction{ 0.0f };
		float CutOff = 0.0f;

		SpotLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const glm::vec3& position, const Camera& camera, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].position", position);
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].direction", Direction);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].cutoff", CutOff);
			shader->SetFloat3("u_ViewPos", camera.GetPosition());
		}
	};
}