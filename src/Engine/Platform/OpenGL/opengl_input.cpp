#ifdef CHAF_OPENGL_API

#include <Engine/log.h>
#include <Engine/core.h>
#include <Engine/input_system.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Chaf
{
	template<>
	bool Input::IsKeyPressed<GLFWwindow>(GLFWwindow* window, KeyCode keycode)
	{
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	template<>
	bool Input::IsMouseButtonPressed<GLFWwindow>(GLFWwindow* window, MouseCode button)
	{
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	template<>
	std::pair<float, float> Input::GetMousePosition<GLFWwindow>(GLFWwindow* window)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { float(xpos),float(ypos) };
	}

	template<>
	float Input::GetMouseX<GLFWwindow>(GLFWwindow* window)
	{
		auto [x, y] = GetMousePosition(window);
		return (float)x;
	}

	template<>
	float Input::GetMouseY<GLFWwindow>(GLFWwindow* window)
	{
		auto [x, y] = GetMousePosition(window);
		return (float)y;
	}

	template<>
	void Input::SetCursorHidden<GLFWwindow>(GLFWwindow* window, const bool enable)
	{
		if (enable)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

#endif // CHAF_OPENGL_API