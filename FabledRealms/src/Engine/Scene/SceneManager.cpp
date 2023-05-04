#include "frpch.h"
#include "SceneManager.h"

// Scenes
#include "FabledRealms/Scenes/MainMenuScene.h"
#include "FabledRealms/Scenes/WorldScene.h"


SceneManager* SceneManager::s_Instance = nullptr;

SceneManager::SceneManager()
{
	FR_CORE_ASSERT(!SceneManager::s_Instance, "Scene Manager Already Exists!");
	SceneManager::s_Instance = this;
	SceneManager::currentScene = nullptr;
}

void SceneManager::UpdateScene(const Time& const time)
{
	FR_CORE_ASSERT(SceneManager::currentScene != nullptr, "Scene Manager's Current Scene is null!");
	currentScene->Update(time);
}

void SceneManager::SwitchScene(Scenes scene)
{
	if (currentScene != nullptr)
		delete currentScene;

	currentScene = nullptr;

	switch (scene)
	{

	case SceneManager::MENU:
		currentScene = new MainMenuScene();
		break;

	case SceneManager::WORLD:
		currentScene = new WorldScene();
		break;

	default:
		FR_CORE_ASSERT(false, "Unknown Scene ID");
	}
}
