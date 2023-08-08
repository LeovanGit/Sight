#include <iostream>
#include <windows.h>
#include <memory>

#include "translucent_window.h"
#include "menu_window.h"
#include "texture.h"

// TODO: think about smarter architecture :d
std::unique_ptr<MenuWindow> menuWin;

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

#ifdef LOGS
    std::cout << "Screen size: " << screenWidth << "x" << screenHeight << "\n";
#endif

    menuWin = std::make_unique<MenuWindow>(hInstance,
                                           400,
                                           400,
                                           10,
                                           10);
    
    //menuWin->crosshairWin->Draw();
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

