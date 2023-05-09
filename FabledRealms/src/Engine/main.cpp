#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"
#include "Engine/Time.h"

#include "Scene/SceneManager.h"
#include "Sound/AudioManager.h"

extern Application* CreateApplication();

int main()
{
    AudioManager* audioManager = new AudioManager();
    SceneManager* sceneManager = new SceneManager();
   

    Application* app = CreateApplication();
    app->Run();
    delete app;

    delete sceneManager;
    delete audioManager;

    return 0;
}