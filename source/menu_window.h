#pragma once

#include <memory>

#include "window.h"
#include "translucent_window.h"

class MenuWindow : public IWindow
{
public:
    MenuWindow(HINSTANCE hInstance,
               uint32_t width,
               uint32_t height,
               uint32_t posX,
               uint32_t posY);
    
    // temporarily unavailable
    // deleted methods should be public for better error messages
    MenuWindow(MenuWindow &) = delete;
    MenuWindow(MenuWindow &&) = delete;
    void operator=(MenuWindow &) = delete;
    void operator=(MenuWindow &&) = delete;

    ~MenuWindow();

    std::unique_ptr<TranslucentWindow> crosshairWin;
    
protected:
    static LRESULT CALLBACK windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
    
    virtual void registerWindowClass(HINSTANCE hInstance);
    virtual void createWindow(HINSTANCE hInstance);

    void initCrosshair(HINSTANCE hInstance);
};
