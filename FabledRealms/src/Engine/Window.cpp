#include "frpch.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Application.h"

//Callback function that glfw calls when the window is resized
void OnWindowResize(GLFWwindow* window, int newWidth, int newHeight)
{
	

	Application::Get().OnWindowResized(newWidth, newHeight);
}

//From https://www.khronos.org/opengl/wiki/OpenGL_Error

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);

	__debugbreak();
//	FR_CORE_ASSERT(false, "OpenGL Error!");
}

//#define FULLSCREEN


Window::Window(const char* title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;

	//Initialize glfw
	int status = glfwInit();
	FR_CORE_ASSERT(status, "Failed to initialize GLFW");


	//Opengl 4.6
	
	//Window Hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//Create Window
#ifdef FULLSCREEN
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, glfwGetPrimaryMonitor(), NULL);
#else // FULLSCREEN
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
#endif
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

	//Set OpenGL Debug Callback
#ifdef FR_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif
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

void Window::Resize(int width, int height)
{
	//Update width and height
	SetWidthAndHeight(width, height);

	//Update the opengl viewport
	glViewport(0, 0, width, height);
}
