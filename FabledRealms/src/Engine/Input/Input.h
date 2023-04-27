#pragma once
#include <glm/glm.hpp>
#include "KeyCodes.h"

class Input
{
public:
	enum class MouseMode
	{
		NORMAL,
		HIDDEN,
		CAPTURED,
		DISABLED, 
	};

	static void SetMouseMode(MouseMode mode);

	static bool IsKeyPressed(int keycode);
	static glm::vec2 GetMousePosition();
};