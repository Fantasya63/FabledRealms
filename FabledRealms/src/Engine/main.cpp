#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"
#include "Engine/Time.h"

extern Application* CreateApplication();

int main()
{
    Window window("Fabled Realms", 1200, 720);
    Application* app = CreateApplication();


    Time time;
  
    double timeSinceLastFrame = time.currentTime;

    while (!window.WindowShouldClose())
    {
        time.currentTime = window.GetCurrentTime();
        time.deltaTime = time.currentTime - timeSinceLastFrame;
        timeSinceLastFrame = time.currentTime;

        // input
        // -----
        

        // render
        // ------
        app->OnUpdate(time);


        window.WindowSwapBuffers();
        window.WindowPollEvents();
    }

    delete app;
    return 0;
}