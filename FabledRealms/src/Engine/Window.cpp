#include "frpch.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void OnWindowResize(GLFWwindow* window, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}

Window::Window(const char* title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;

	int status = glfwInit();
	FR_CORE_ASSERT(status, "Failed to initialize GLFW");

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);

	glfwMakeContextCurrent((GLFWwindow*)m_Window);
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

bool Window::WindowShouldClose()
{
	return glfwWindowShouldClose((GLFWwindow*) m_Window);
}

void Window::WindowPollEvents()
{
	glfwPollEvents();
}

void Window::WindowSwapBuffers()
{
	glfwSwapBuffers((GLFWwindow*) m_Window);
}

double Window::GetCurrentTime()
{
	return glfwGetTime();
}
