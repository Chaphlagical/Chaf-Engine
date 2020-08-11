#include <Engine/log.h>
#include <Engine/core.h>
#include <Renderer/renderer.h>
#include <Renderer/shader.h>

#ifdef CHAF_OPENGL_API
#include <Renderer/Platform/OpenGL/opengl_shader.h>
#endif // CHAF_OPENGL_API

namespace Chaf
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLShader>(filepath);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		CHAF_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		m_Shaders;
		CHAF_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}