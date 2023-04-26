#pragma once
#include "Engine/Time.h"
#include "Window.h"

class Application
{
public:
	Application();
	virtual ~Application();

	void Run();
	Window* GetWindow() { return m_Window; }

	static Application& Get() { return *s_Instance; }

protected:
	virtual void OnUpdate(const Time& time) {};

private:
	Window* m_Window;
	bool m_Running = true;

	static Application* s_Instance;
};