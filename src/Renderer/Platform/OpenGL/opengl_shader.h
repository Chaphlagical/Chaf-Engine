#pragma once

#include <Renderer/shader.h>
#include <glad/glad.h>

namespace Chaf
{
	class CHAF_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetBool(const std::string& name, const bool& value) override;
		virtual void SetFloat(const std::string& name, const float& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetInt(const std::string& name, const int& value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformMat4(const std::string name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string name, const glm::mat3& matrix);

		void UploadUniformInt(const std::string name, const int& value);

		void UploadUniformBool(const std::string name, const bool& value);

		void UploadUniformFloat1(const std::string name, const float& value);
		void UploadUniformFloat2(const std::string name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string name, const glm::vec4& value);

	private:
		std::string ReadFile(const std::string filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
	};

}