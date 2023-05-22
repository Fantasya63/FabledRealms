#pragma once
#include <glm/glm.hpp>
#include "KeyCodes.h"

class InputManager
{
public:
	InputManager();
	~InputManager();


	enum class MouseMode
	{
		NORMAL,
		HIDDEN,
		CAPTURED,
		DISABLED, 
	};

	static InputManager& Get() { return *s_Instance; }

	static void SetMouseMode(MouseMode mode);

	static bool IsKeyDown(int keycode);
	static bool IsMouseButtonDown(int keycode);

	void SetMouseButtonCallback(void(*callback)(int, int, int));
	void SetKeyboardButtonCallback(void(*callback)(int, int, int, int));

	static glm::vec2 GetMousePosition();

private:
	static InputManager* s_Instance;

};