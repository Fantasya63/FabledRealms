#include "frpch.h"
#include "Input.h"
#include "Engine/Application.h"
#include <GLFW/glfw3.h>

void(*MouseCallback)(int, int, int) = nullptr;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (MouseCallback != nullptr)
		MouseCallback(button, action, mods);
}

InputManager* InputManager::s_Instance = nullptr;

InputManager::InputManager()
{
	FR_CORE_ASSERT(!s_Instance, "InputManager Already Exists!");
	s_Instance = this;

	//Setup Mouse button callback funtion
	void* window = Application::Get().GetWindow()->GetWindowHandle();
	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(window), MouseButtonCallback);
}

void InputManager::SetMouseMode(MouseMode mode)
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


bool InputManager::IsKeyDown(int keycode)
{
	void* window = Application::Get().GetWindow()->GetWindowHandle();
	int state = glfwGetKey(static_cast<GLFWwindow*>(window), keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputManager::IsMouseButtonDown(int keycode)
{
	void* window = Application::Get().GetWindow()->GetWindowHandle();
	int state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

void InputManager::SetMouseButtonCallback(void(*callback)(int, int, int))
{
	MouseCallback = callback;
}

glm::vec2 InputManager::GetMousePosition()
{
	double x, y;

	void* window = Application::Get().GetWindow()->GetWindowHandle();
	glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x, &y);

	return glm::vec2(x, y);
}
