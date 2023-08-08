#include "menu_window.h"

extern std::unique_ptr<MenuWindow> menuWin;

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

    initCrosshair(hInstance);
}

LRESULT CALLBACK MenuWindow::windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        // if menu is closed than we should exit app
        PostQuitMessage(0);
        return 0;
    }
    case WM_DROPFILES:
    {
        HDROP hDrop = reinterpret_cast<HDROP>(wParam);
        
        std::string filename;
        // get path size of the first file (0) and ignore other:
        uint32_t size = DragQueryFileA(hDrop, 0, NULL, 0);
        if (size > 0)
        {
            filename.resize(size);
            DragQueryFileA(hDrop, 0, &filename[0], size + 1);

            Texture crosshairTexture(filename);
            menuWin->crosshairWin->setTexture(&crosshairTexture);
            menuWin->crosshairWin->Draw();
            ShowWindow(menuWin->crosshairWin->getHandle(), SW_SHOW);
        }
        
        DragFinish(hDrop);
        
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

void MenuWindow::initCrosshair(HINSTANCE hInstance)
{
    crosshairWin = std::make_unique<TranslucentWindow>(hInstance,
                                                       10,
                                                       10,
                                                       0,
                                                       0);    
}

MenuWindow::~MenuWindow() {}
