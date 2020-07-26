#include <Renderer/renderAPI.h>
#include <Renderer/renderer.h>
#include <Renderer/effect.h>
#include <Renderer/Platform/OpenGL/opengl_effect.h>

namespace Chaf
{
	Ref<Cubemap> Cubemap::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLCubemap>(path);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<Cubemap> Cubemap::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLCubemap>();
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}
}