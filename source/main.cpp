#include <iostream>
#include <windows.h>

#include "translucent_window.h"
#include "menu_window.h"
#include "texture.h"

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

    Texture crosshairTexture("./assets/crosshair.png");

    uint32_t winWidth = crosshairTexture.getWidth();
    uint32_t winHeight = crosshairTexture.getHeight();
    
    TranslucentWindow win(hInstance,
                          winWidth,
                          winHeight,
                          (screenWidth - winWidth) / 2, // center of the screen
                          (screenHeight - winHeight) / 2);
    
    win.setTexture(&crosshairTexture);
    win.Draw();

    MenuWindow menuWin(hInstance,
                       400,
                       400,
                       10,
                       10);
    
    MSG msg;
    // no need PeekMessage here, because of 1 draw call
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

