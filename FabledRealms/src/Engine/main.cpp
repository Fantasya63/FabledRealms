#include "frpch.h"

#include "Engine/Application.h"
#include "Engine/Window.h"

extern Application* CreateApplication();

int main()
{
    Window window("Fabled Realms", 1200, 720);
    Application* app = CreateApplication();

    // render loop
    // -----------

    while (!window.WindowShouldClose())
    {
        // input
        // -----
        

        // render
        // ------
        app->OnUpdate(1.0f);


        window.WindowSwapBuffers();
        window.WindowPollEvents();
    }

    delete app;
    return 0;
}