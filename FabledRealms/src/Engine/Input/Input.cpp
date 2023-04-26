#include "frpch.h"
#include "Input.h"
#include "Engine/Application.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(int keycode)
{
	void* window = Application::Get().GetWindow()->GetWindowHandle();
	int state = glfwGetKey(static_cast<GLFWwindow*>(window), keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
