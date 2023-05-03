#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"
#include "Engine/Time.h"

#include "Scene/SceneManager.h"

extern Application* CreateApplication();

int main()
{
    SceneManager* sceneManager = new SceneManager();

    Application* app = CreateApplication();
    app->Run();
    delete app;

    delete sceneManager;

    return 0;
}