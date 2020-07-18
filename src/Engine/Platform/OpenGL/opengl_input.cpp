#ifdef CHAF_OPENGL_API

#include <Engine/log.h>
#include <Engine/core.h>
#include <Engine/input_system.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Chaf
{
	
	bool Input::IsKeyPressed(void* window, KeyCode keycode)
	{
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window), static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	
	bool Input::IsMouseButtonPressed(void* window, MouseCode button)
	{
		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	
	std::pair<float, float> Input::GetMousePosition(void* window)
	{
		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window), &xpos, &ypos);
		return { float(xpos),float(ypos) };
	}

	
	float Input::GetMouseX(void* window)
	{
		auto [x, y] = GetMousePosition(static_cast<GLFWwindow*>(window));
		return (float)x;
	}

	
	float Input::GetMouseY(void* window)
	{
		auto [x, y] = GetMousePosition(static_cast<GLFWwindow*>(window));
		return (float)y;
	}

	
	void Input::SetCursorHidden(void* window, const bool enable)
	{
		if (enable)
		{
			glfwSetInputMode(static_cast<GLFWwindow*>(window), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
			glfwSetInputMode(static_cast<GLFWwindow*>(window), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

#endif // CHAF_OPENGL_API