#include "frpch.h"
#include "Application.h"
#include "Input/Input.h"
#include <Engine/Scene/SceneManager.h>

Application* Application::s_Instance = nullptr;

Application::Application()
{
	//Make sure there is only one Application
	FR_CORE_ASSERT(!s_Instance, "Application Already Exists!");
	s_Instance = this;

	m_Window = new Window("Fabled Realms", 1280, 720);

	//Create the Window first then create the input Manager as this depends on the Window class
	InputManager* inputManager = new InputManager();
}

Application::~Application()
{
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

		//Calculate the time elapsed from last frame to this frame
		time.deltaTime = time.currentTime - timeSinceLastFrame;

		//Update time since last frame
		timeSinceLastFrame = time.currentTime;

		OnUpdate(time);

		m_Window->OnUpdate();

		//Resize
		if (m_ResizeWindow)
		{
			m_Window->Resize(m_NewResolution.x, m_NewResolution.y);
			SceneManager::Get().OnWindowResized(m_NewResolution.x, m_NewResolution.y);
			m_ResizeWindow = false;
		}

		if (m_Window->WindowShouldClose())
			m_Running = false;
	}
}

void Application::RequestClose()
{
	m_Running = false;
}

void Application::OnWindowResized(int width, int height)
{
	//Delay Resize till the end of frame
	m_ResizeWindow = true;
	m_NewResolution = glm::ivec2(width, height);
}
