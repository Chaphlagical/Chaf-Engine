#include <Renderer/buffer.h>
#include <Renderer/renderer.h>
#ifdef CHAF_OPENGL_API
#include <Renderer/Platform/OpenGL/opengl_buffer.h>
#endif // CHAF_OPENGL_API

namespace Chaf
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLVertexBuffer>(size);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RendererAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			CHAF_CORE_INFO("RendererAPI: OpenGL");
			return CreateRef<OpenGLFrameBuffer>(spec);
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RendererAPI::API!");
		return nullptr;
	}
}