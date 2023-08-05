#pragma once

#include "window.h"

class TranslucentWindow : public IWindow
{
 public:
    TranslucentWindow(HINSTANCE hInstance,
                      uint32_t width,
                      uint32_t height,
                      uint32_t posX,
                      uint32_t posY);
    
    void setTexture(const Texture * texture);
    void Draw();

 protected:
    virtual LRESULT CALLBACK windowProc(HWND hWin,
                                        UINT message,
                                        WPARAM wParam,
                                        LPARAM lParam);

    virtual void registerWindowClass(HINSTANCE hInstance);
    virtual void createWindow(HINSTANCE hInstance);

    
    void initBlendFunc();
    void initBitmapInfo();
    void initPixelsBuffer();

    BLENDFUNCTION blendFunc;

    HDC hdcBitmap;
    HBITMAP hBitmap;
    BITMAPINFO bmi;
    // TODO: use heap, std::vector for example:
    void * pixels;
};

    /* // temporarily unavailable */
    /* // deleted methods should be public for better error messages */
    /* IWindow(IWindow &) = delete; */
    /* IWindow(IWindow &&) = delete; */
    /* void operator=(IWindow &) = delete; */
    /* void operator=(IWindow &&) = delete; */
