#include "frpch.h"
#include "SceneManager.h"

// Scenes
#include "FabledRealms/Scenes/MainMenuScene.h"
#include "FabledRealms/Scenes/WorldScene.h"


SceneManager* SceneManager::s_Instance = nullptr;

SceneManager::SceneManager()
{
	//Make sure there is only one instance
	FR_CORE_ASSERT(!SceneManager::s_Instance, "Scene Manager Already Exists!");

	SceneManager::s_Instance = this;
	SceneManager::currentScene = nullptr;
}

SceneManager::~SceneManager()
{
	//Delete current scene if it exist
	if (currentScene != nullptr)
		delete currentScene;
}

void SceneManager::UpdateScene(const Time& time)
{
	//Make sure we have a scene that we're updating
	FR_CORE_ASSERT(SceneManager::currentScene != nullptr, "Scene Manager's Current Scene is null!");

	currentScene->Update(time);
}

void SceneManager::SwitchScene(Scenes scene)
{
	//Check if we have a scene that we're switching from
	if (currentScene != nullptr)
		//Delete the scene
		delete currentScene;

	currentScene = nullptr;

	//Switch to a new Scene depending on which scene
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
