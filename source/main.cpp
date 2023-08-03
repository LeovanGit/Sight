#include <iostream>
#include <windows.h>

#include "window.h"
#include "texture.h"

namespace
{
uint32_t winWidth = 100;
uint32_t winHeight = 100;
}

LRESULT CALLBACK WindowProc(HWND hWin,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

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

    Texture sightTexture("./assets/cross.png");

    winWidth = sightTexture.width;
    winHeight = sightTexture.height;
    
    Window win(hInstance,
               WindowProc,
               winWidth,
               winHeight,
               (screenWidth - winWidth) / 2,
               (screenHeight - winHeight) / 2);

    win.setTexture(&sightTexture);
    win.Draw();
    
    MSG msg;
    // while (true)
    // {
    //     if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //     {
    //         TranslateMessage(&msg);
    //         DispatchMessage(&msg);

    //         if (msg.message == WM_QUIT) break;
    //     }
    //     else
    //     {
    //         // TODO: add FPS limitation
    //         // we should redraw now, because of alphablend
    //         // (we should blend each frame with background)
    //         win.Draw();
    //     }
    // }
    
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hWin, message, wParam, lParam);
}
