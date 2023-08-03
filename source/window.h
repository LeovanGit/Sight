#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include "wingdi.h"

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

    HWND hWin;
    HDC hdc;    

    void setTexture(const Texture * texture);
    void Draw();

    ~Window();
    
 private:
    void initWindowClass(HINSTANCE hInstance, WNDPROC windowProc);
    void initPixelsBuffer();
    
    uint32_t width;
    uint32_t height;

    uint32_t posX;
    uint32_t posY;

    BITMAPINFO bmi;
    std::vector<uint32_t> pixels;

    BLENDFUNCTION bf;
    HDC hdcBitmap;
    HBITMAP hbitmap;
    VOID *pvBits;
};
