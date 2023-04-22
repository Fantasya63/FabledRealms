#include "frpch.h"
#include "FabledRealmsApp.h"
#include "glad/glad.h"

Application* CreateApplication()
{
	return new FabledRealmsApp();
}

FabledRealmsApp::FabledRealmsApp()
{
}

FabledRealmsApp::~FabledRealmsApp()
{
	LOG_INFO("FABLED REALMS EXIT");
}

void FabledRealmsApp::OnUpdate(float deltaTime)
{
	LOG_INFO("HELLO WORLD!");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
