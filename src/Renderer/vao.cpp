#include <Renderer/vao.h>
#include <Renderer/renderer.h>
#ifdef CHAF_OPENGL_API
#include <Renderer/Platform/OpenGL/opengl_vao.h>
#endif // CHAF_OPENGL_API

namespace Chaf
{
	Ref<VertexArray> VertexArray::Create()
	{

		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:
			CHAF_CORE_ASSERT(false, "RenderAPI::API::None is not currently not supported!");
			return nullptr;
		case RenderAPI::API::OpenGL:
			//CHAF_CORE_INFO("RenderAPI: OpenGL");
			return CreateRef<OpenGLVertexArray>();
		default:
			break;
		}
		CHAF_CORE_ASSERT(false, "Unknown RenderAPI::API!");
		return nullptr;
	}

}