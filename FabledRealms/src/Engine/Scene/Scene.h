#pragma once
#include "Engine/Time.h"

class Scene
{
public:
	virtual void Update(const Time& const time) = 0;
};