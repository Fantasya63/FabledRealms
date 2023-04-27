#include "frpch.h"
#include "Input.h"
#include "Engine/Application.h"
#include <GLFW/glfw3.h>

void Input::SetMouseMode(MouseMode mode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());

	int mouseMode;
	switch (mode)
	{
	case MouseMode::NORMAL:
		mouseMode = GLFW_CURSOR_NORMAL;
		break;

	case MouseMode::HIDDEN:
		mouseMode = GLFW_CURSOR_HIDDEN;
		break;

	case MouseMode::CAPTURED:
		mouseMode = GLFW_CURSOR_CAPTURED;
		break;

	case MouseMode::DISABLED:
		mouseMode = GLFW_CURSOR_DISABLED;
		break;

	default:
		FR_CORE_ASSERT(false, "Unknown MouseMode!");
	}

	 glfwSetInputMode(window, GLFW_CURSOR, mouseMode);
}

bool Input::IsKeyPressed(int keycode)
{
	void* window = Application::Get().GetWindow()->GetWindowHandle();
	int state = glfwGetKey(static_cast<GLFWwindow*>(window), keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

glm::vec2 Input::GetMousePosition()
{
	double x, y;

	void* window = Application::Get().GetWindow()->GetWindowHandle();
	glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x, &y);

	return glm::vec2(x, y);
}

