#include "frpch.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application.h"

//Callback function that glfw calls when the window is resized
void OnWindowResize(GLFWwindow* window, int newWidth, int newHeight)
{
	//Update width and height
	Application::Get().GetWindow()->SetWidthAndHeight(newWidth, newHeight);

	//Update the opengl viewport
	glViewport(0, 0, newWidth, newHeight);
}


Window::Window(const char* title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;

	//Initialize glfw
	int status = glfwInit();
	FR_CORE_ASSERT(status, "Failed to initialize GLFW");

	//Create Window
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	
	//Make the opengl context to current
	glfwMakeContextCurrent((GLFWwindow*)m_Window);

	//Tell which function to call whenever the window resizes
	glfwSetFramebufferSizeCallback((GLFWwindow*)m_Window, OnWindowResize);

	//Load OpenGl functions
	status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	FR_ASSERT(status, "Failed To Initialize glad")


	LOG_CORE_INFO("Initialized OpenGL Context");
	LOG_CORE_INFO("  Vendor: " << (const char*)glGetString(GL_VENDOR));
	LOG_CORE_INFO("  Renderer: " << (const char*)glGetString(GL_RENDERER));
	LOG_CORE_INFO("  OpenGl version: " << (const char*)glGetString(GL_VERSION));

	SetVSync(true);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::SetVSync(bool enabled)
{
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_VSync = enabled;
}

void Window::OnUpdate()
{
	glfwPollEvents();

	//Show the rendered frame to the monitor
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
}


double Window::GetCurrentTime()
{
	return glfwGetTime();
}

bool Window::WindowShouldClose()
{
	//Returns true if the close button is pressed on the window
	return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window));
}
