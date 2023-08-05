#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <wingdi.h>

#include "texture.h"

class IWindow
{
public:
    IWindow(HINSTANCE hInstance,
            uint32_t width,
            uint32_t height,
            uint32_t posX,
            uint32_t posY);

    virtual ~IWindow();

    // temporarily unavailable
    // deleted methods should be public for better error messages
    IWindow(IWindow &) = delete;
    IWindow(IWindow &&) = delete;
    void operator=(IWindow &) = delete;
    void operator=(IWindow &&) = delete;
    
protected:
    virtual void registerWindowClass(HINSTANCE hInstance) = 0;
    virtual void createWindow(HINSTANCE hInstance) = 0;
    
    HWND hWin;
    HDC hdcWin;
    
    uint32_t width;
    uint32_t height;

    uint32_t posX;
    uint32_t posY;
};
