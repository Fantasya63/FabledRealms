#pragma once
#include "Engine/Time.h"

class Scene
{
public:
	virtual ~Scene() {}

	virtual void Update(const Time& const time) = 0;
};