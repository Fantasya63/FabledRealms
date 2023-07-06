#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"
#include "Engine/Time.h"

#include "Scene/SceneManager.h"
#include "Sound/AudioManager.h"
#include "Engine/Graphics/TextureAtlass.h"

extern Application* CreateApplication();

int main()
{
    AudioManager* audioManager = new AudioManager();
    SceneManager* sceneManager = new SceneManager();
   
    TextureAtlass::Init();

    Application* app = CreateApplication();
    app->Run();
    delete app;

    delete sceneManager;
    delete audioManager;

    return 0;
}