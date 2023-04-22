#pragma once
#include "Engine/Time.h"

class Application
{
public:
	virtual ~Application() {};

	virtual void OnUpdate(const Time& time) {};

private:
	bool m_Running = true;
};