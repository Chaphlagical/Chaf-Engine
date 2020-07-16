#include <Renderer/texture.h>
#include <Renderer/Renderer.h>

#include <Renderer/Platform/OpenGL/opengl_texture.h>

namespace Chaf
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLTexture2D>(path);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLTexture2D>(width, height);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

}