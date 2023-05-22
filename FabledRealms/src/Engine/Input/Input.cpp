#include "frpch.h"
#include "Input.h"
#include "Engine/Application.h"
#include <GLFW/glfw3.h>


// ----------------------- Input Callbacks --------------------------------------

// Declare function pointers for our callbacks that will be called when making an input
void(*MouseCallback)(int, int, int) = nullptr;
void(*KeyboardCallback)(int, int, int, int) = nullptr;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//Check if mouse callback is set
	if (MouseCallback != nullptr)
		//Call keyboard callback
		MouseCallback(button, action, mods);
}

void KeyboardButtonCallback(GLFWwindow* window, int button, int scancode, int action, int mods)
{
	//Check if keyboard callback is set
	if (KeyboardCallback != nullptr)
		//Call keyboard callback
		KeyboardCallback(button, scancode, action, mods);
}

//---------------------------------------------------------------------------------

InputManager* InputManager::s_Instance = nullptr;

InputManager::InputManager()
{
	FR_CORE_ASSERT(!s_Instance, "InputManager Already Exists!");
	s_Instance = this;

	//Setup Mouse and Keyboard button and callback funtions
	void* window = Application::Get().GetWindow()->GetWindowHandle();

	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(window), MouseButtonCallback);
	glfwSetKeyCallback(static_cast<GLFWwindow*>(window), KeyboardButtonCallback);
}

void InputManager::SetMouseMode(MouseMode mode)
{
	//Get the window Handle
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetWindowHandle());

	//Map our mouse mode class enum into glfw's
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
		break;
	}

	//Set the mouse mode with the mapped state
	glfwSetInputMode(window, GLFW_CURSOR, mouseMode);
}


bool InputManager::IsKeyDown(int keycode)
{
	//Get the window Handle
	void* window = Application::Get().GetWindow()->GetWindowHandle();

	//Get the state of the key using the window handle
	int state = glfwGetKey(static_cast<GLFWwindow*>(window), keycode);

	//return true if it is pressed or being hold down
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputManager::IsMouseButtonDown(int keycode)
{
	//Get the window Handle
	void* window = Application::Get().GetWindow()->GetWindowHandle();

	//Get the state of the mouse button using the window handle 
	int state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), keycode);

	//return true if it is pressed or being hold down
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

void InputManager::SetMouseButtonCallback(void(*callback)(int, int, int))
{
	MouseCallback = callback;
}

void InputManager::SetKeyboardButtonCallback(void(*callback)(int, int, int, int))
{
	KeyboardCallback = callback;
}

glm::vec2 InputManager::GetMousePosition()
{
	double x, y;

	//Get the Window Handle
	void* window = Application::Get().GetWindow()->GetWindowHandle();

	//Get the cursor position using the window Handle
	glfwGetCursorPos(static_cast<GLFWwindow*>(window), &x, &y);

	//Return the resulting value in a vec2
	return glm::vec2(x, y);
}
