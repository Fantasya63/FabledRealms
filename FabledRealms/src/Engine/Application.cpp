#include "frpch.h"
#include "Application.h"
#include "Input/Input.h"


Application* Application::s_Instance = nullptr;

Application::Application()
{
	FR_CORE_ASSERT(!s_Instance, "Application Already Exists!");
	s_Instance = this;

	m_Window = new Window("Fabled Realms", 1280, 720);
	InputManager* inputManager = new InputManager();
}

Application::~Application()
{
	DLOG_CORE_INFO("Base Application Destructor");
	delete m_Window;
}

void Application::Run()
{
	Time time;
	time.currentTime = m_Window->GetCurrentTime();
	float timeSinceLastFrame = time.currentTime;

	while (m_Running)
	{
		time.currentTime = m_Window->GetCurrentTime();
		time.deltaTime = time.currentTime - timeSinceLastFrame;
		timeSinceLastFrame = time.currentTime;

		OnUpdate(time);

		m_Window->OnUpdate();

		if (m_Window->WindowShouldClose())
			m_Running = false;
	}
}

void Application::RequestClose()
{
	m_Running = false;
}
