#include <windows.h>

#include "MazeWindow.h"


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
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {        

        MazeWindow app;

        app.setModel(new MazeModel);

        if (app.init())
            app.RunMessageLoop();

        CoUninitialize();
    }

    exit(0);
}