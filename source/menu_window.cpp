#include "menu_window.h"

MenuWindow::MenuWindow(HINSTANCE hInstance,
                       uint32_t width,
                       uint32_t height,
                       uint32_t posX,
                       uint32_t posY) :
                       IWindow(hInstance,
                               width,
                               height,
                               posX,
                               posY)
{
    registerWindowClass(hInstance);
    createWindow(hInstance);

    ShowWindow(hWin, SW_SHOW);
}

LRESULT CALLBACK MenuWindow::windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
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

void MenuWindow::registerWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX winDesc;
    ZeroMemory(&winDesc, sizeof(WNDCLASSEX));
    
    winDesc.cbSize = sizeof(WNDCLASSEX);
    winDesc.style = CS_HREDRAW | CS_VREDRAW;
    winDesc.lpfnWndProc = windowProc;
    winDesc.hInstance = hInstance;
    winDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winDesc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    winDesc.lpszClassName = "WindowClassMenu";

    RegisterClassEx(&winDesc);
}

void MenuWindow::createWindow(HINSTANCE hInstance)
{
    hWin = CreateWindowEx(WS_EX_ACCEPTFILES,
                          "WindowClassMenu",
                          "Menu",
                          WS_OVERLAPPEDWINDOW,
                          posX,
                          posY,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    
    hdcWin = GetDC(hWin);
}

MenuWindow::~MenuWindow() {}
