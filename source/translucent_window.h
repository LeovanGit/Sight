#pragma once

#include "window.h"

class TranslucentWindow : public IWindow
{
 public:
    TranslucentWindow(HINSTANCE hInstance);

    HBITMAP getBitmap() const;
    
    void setTexture(const Texture * texture);
    void reset();
    void draw();
    
    // resize by mouse
    virtual void onResize(uint32_t newWidth, uint32_t newHeight);
    
    ~TranslucentWindow();

    // temporarily unavailable
    // deleted methods should be public for better error messages
    TranslucentWindow(TranslucentWindow &) = delete;
    TranslucentWindow(TranslucentWindow &&) = delete;
    void operator=(TranslucentWindow &) = delete;
    void operator=(TranslucentWindow &&) = delete;
    
 protected:
    virtual LRESULT CALLBACK windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK messageRouter(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
    
    virtual void registerWindowClass(HINSTANCE hInstance);
    virtual void createWindow(HINSTANCE hInstance);
    
    void initBlendFunc();
    void initBitmapInfo();
    
    void resetPixelsBuffer();
    void resetPixelsBufferWithChessPattern();
    
    BLENDFUNCTION blendFunc;
    
    HDC hdcBitmap;
    HBITMAP hBitmap;
    BITMAPINFO bmi;
    // TODO: use heap, std::vector for example:
    void * pixels;

    static const uint32_t defaultWidth = 14;
    static const uint32_t defaultHeight = 14;
};
