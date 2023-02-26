#include <windows.h>

#include "MazeWindow.h"
#include "GameController.h"

int 
#if !defined(_MAC)
    #if defined(_M_CEE_PURE)
        __clrcall
    #else
        WINAPI
    #endif
#else
    CALLBACK
#endif
WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    try {
        if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {

            MazeWindow app;

            GameController controller;
            std::thread controllerRunner(&GameController::run, &controller);
            app.setModel(controller.model());

            app.RunMessageLoop();
            controller.setState(false);
            controllerRunner.join();
            CoUninitialize();
        }
    }
    catch (const std::exception& ex) {
        MessageBoxA(0, ex.what(), "Fatal Error", MB_OK | MB_ICONERROR);
    }

    exit(0);
}