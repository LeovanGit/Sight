#include <iostream>
#include <windows.h>
#include <memory>

#include "translucent_window.h"
#include "menu_window.h"
#include "texture.h"

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
#ifdef LOGS
    std::cout << "Screen size: " << screenWidth << "x" << screenHeight << "\n";
#endif

    MenuWindow menuWin(hInstance,
                       400,
                       400,
                       10,
                       10);

    menuWin.showWindow();
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

