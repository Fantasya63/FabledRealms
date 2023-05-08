#include "frpch.h"
#include "FabledRealmsApp.h"
#include "Engine/Input/Input.h"
#include "Engine/Scene/SceneManager.h"

Application* CreateApplication()
{
	return new FabledRealmsApp();
}

FabledRealmsApp::FabledRealmsApp()
{
    //Load Scene
    SceneManager::Get().SwitchScene(SceneManager::MENU);
}

FabledRealmsApp::~FabledRealmsApp()
{
	LOG_INFO("FABLED REALMS EXIT");
}

void FabledRealmsApp::OnUpdate(const Time& time)
{
    //Update The Scene
    SceneManager::Get().UpdateScene(time);
}
