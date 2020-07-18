#ifdef CHAF_OPENGL_API
#include <Engine/time.h>

#include <GLFW/glfw3.h>

namespace Chaf
{
	void Timestep::Update(float lastFrameTime)
	{
		m_Time = (float)glfwGetTime() - lastFrameTime;
	}
}
#endif // CHAF_OPENGL_API