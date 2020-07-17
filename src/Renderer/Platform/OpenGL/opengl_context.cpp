#include <Engine/log.h>
#include <Engine/core.h>

#include <Renderer/Platform/OpenGL/opengl_context.h>

#include<glad/glad.h>
#include <GLFW/glfw3.h>

namespace Chaf
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		CHAF_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CHAF_CORE_ASSERT(status, "Failed to initialize Glad!");

		CHAF_CORE_INFO("OpenGL Info:");
		CHAF_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		CHAF_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		CHAF_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}