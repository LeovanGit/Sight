#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <wingdi.h>

#include "texture.h"

class Window
{
 public:
    Window(HINSTANCE hInstance,
           WNDPROC windowProc,
           uint32_t width,
           uint32_t height,
           uint32_t posX,
           uint32_t posY);

    void setTexture(const Texture * texture);
    void Draw();

    ~Window();

    // temporarily unavailable
    // deleted methods should be public for better error messages
    Window(Window &) = delete;
    Window(Window &&) = delete;
    void operator=(Window &) = delete;
    void operator=(Window &&) = delete;
    
 private:
    void initWindowClass(HINSTANCE hInstance, WNDPROC windowProc);
    void createWindow(HINSTANCE hInstance);
    void initBlendFunc();
    void initBitmapInfo();
    void initPixelsBuffer();

    HWND hWin;
    HDC hdcWin;
    
    uint32_t width;
    uint32_t height;

    uint32_t posX;
    uint32_t posY;

    BLENDFUNCTION blendFunc;

    HDC hdcBitmap;
    HBITMAP hBitmap;
    BITMAPINFO bmi;
    // TODO: use heap, std::vector for example:
    void * pixels;
};
