#pragma once
#include "Engine/Time.h"
#include "Window.h"
#include <glm/glm.hpp>

class Application
{
public:
	Application();
	virtual ~Application();

	void Run();
	void RequestClose();

	Window* GetWindow() { return m_Window; }

	static Application& Get() { return *s_Instance; }

	void OnWindowResized(int width, int height);

protected:
	virtual void OnUpdate(const Time& time) {};

private:
	Window* m_Window;
	bool m_Running = true;
	bool m_ResizeWindow = false;
	glm::ivec2 m_NewResolution;

	static Application* s_Instance;
};