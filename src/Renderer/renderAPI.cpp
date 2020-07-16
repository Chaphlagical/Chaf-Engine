#include <Renderer/renderAPI.h>

#ifdef CHAF_OPENGL_API
	Chaf::RenderAPI::API Chaf::RenderAPI::s_API = Chaf::RenderAPI::API::OpenGL;
#endif