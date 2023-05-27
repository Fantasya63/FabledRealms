#pragma once
#include "Scene.h"


class SceneManager
{
public:
	// Container for all of the sce
	enum Scenes
	{
		MENU,
		WORLD,
	};

	static SceneManager& Get() { return *s_Instance; };
	SceneManager();
	~SceneManager();

	void UpdateScene(const Time& time);
	void SwitchScene(Scenes scene);

private:
	static SceneManager* s_Instance;

	Scene* currentScene;
};