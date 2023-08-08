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
        
    initCrosshair(hInstance);
}

LRESULT CALLBACK MenuWindow::windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        // if menu is closed then we should exit app
        PostQuitMessage(0);
        
        return 0;
    }
    case WM_SIZE:
    {
        uint32_t width = LOWORD(lParam);
        uint32_t height = HIWORD(lParam);
        onResize(width, height);
        
        return 0;
    }
    case WM_MOVE:
    {
        uint32_t posX = LOWORD(lParam);
        uint32_t posY = HIWORD(lParam);
        onMove(posX, posY);

        return 0;
    }
    case WM_DROPFILES:
    {
        // handle only first file and ignore other (0)
        HDROP hDrop = reinterpret_cast<HDROP>(wParam);
        
        std::string filename;
        uint32_t size = DragQueryFileA(hDrop, 0, NULL, 0);
        if (size > 0)
        {
            filename.resize(size);
            DragQueryFileA(hDrop, 0, &filename[0], size + 1);

            Texture crosshairTexture(filename);
            crosshairWin->setTexture(&crosshairTexture);
            crosshairWin->draw();
            crosshairWin->showWindow();
        }
        
        DragFinish(hDrop);
        
        return 0;
    }
    }
    
    return DefWindowProc(hWin, message, wParam, lParam);
}

LRESULT CALLBACK MenuWindow::messageRouter(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    MenuWindow *win;

    if (message == WM_CREATE)
    {
        win = static_cast<MenuWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWin, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
    }
    else
    {
        win = reinterpret_cast<MenuWindow*>(GetWindowLongPtr(hWin, GWLP_USERDATA));
    }
 
    if (win)
        return win->windowProc(hWin, message, wParam, lParam);
    else
        // skip all messages before WM_CREATE
        // because on some Windows versions
        // WM_CREATE may not be the first msg
        // after CreateWindowEx() and
        // we will get garbage in win ptr
        return DefWindowProc(hWin, message, wParam, lParam);
}

void MenuWindow::registerWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX desc;
    ZeroMemory(&desc, sizeof(WNDCLASSEX));
    
    desc.cbSize = sizeof(WNDCLASSEX);
    desc.style = CS_HREDRAW | CS_VREDRAW;
    desc.lpfnWndProc = messageRouter;
    desc.hInstance = hInstance;
    desc.hCursor = LoadCursor(NULL, IDC_ARROW);
    desc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    desc.lpszClassName = "WindowClassMenu";
    
    RegisterClassEx(&desc);
}

void MenuWindow::createWindow(HINSTANCE hInstance)
{
    handle = CreateWindowEx(WS_EX_ACCEPTFILES,
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
                            this);
    
    hdc = GetDC(handle);
}

void MenuWindow::initCrosshair(HINSTANCE hInstance)
{
    uint32_t initWidth = 14;
    uint32_t initHeight = 14;
    crosshairWin = std::make_unique<TranslucentWindow>(hInstance,
                                                       initWidth,
                                                       initHeight,
                                                       (screenWidth - initWidth) / 2,
                                                       (screenHeight - initHeight) / 2);
}

MenuWindow::~MenuWindow()
{
    DeleteDC(hdc);
}
