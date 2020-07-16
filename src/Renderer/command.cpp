#ifdef CHAF_OPENGL_API
#include <Renderer/Platform/OpenGL/opengl_renderAPI.h>
#endif // CHAF_OPENGL_API

#include <Renderer/command.h>

namespace Chaf
{
#ifdef CHAF_OPENGL_API
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI;
#endif // CHAF_OPENGL_API
}