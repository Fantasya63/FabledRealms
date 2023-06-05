#pragma once
#include "Engine/Time.h"

class Scene
{
public:
	virtual ~Scene() {}

	virtual void Update(const Time& time) = 0;
	virtual void OnWindowResized(int width, int height) {};
};