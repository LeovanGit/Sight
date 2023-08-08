#pragma once

#include <iostream>
#include <vector>
#include <windows.h>
#include <wingdi.h>

#include "texture.h"

extern int screenWidth;
extern int screenHeight;

class IWindow
{
public:
    IWindow(HINSTANCE hInstance,
            uint32_t width,
            uint32_t height,
            uint32_t posX,
            uint32_t posY);

    HWND getHandle() const;

    void showWindow(bool show = true) const;

    // resize/move from code
    virtual void resize(uint32_t newWidth, uint32_t newHeight);
    virtual void move(uint32_t newPosX, uint32_t newPosY);

    // resize/move by mouse
    virtual void onResize(uint32_t newWidth, uint32_t newHeight);
    virtual void onMove(uint32_t newPosX, uint32_t newPosY);
    
    virtual ~IWindow();

    // temporarily unavailable
    // deleted methods should be public for better error messages
    IWindow(IWindow &) = delete;
    IWindow(IWindow &&) = delete;
    void operator=(IWindow &) = delete;
    void operator=(IWindow &&) = delete;
    
protected:
    // windowProc() can't be class member because of implicit "this" parameter,
    // but we can create static messageRouter() for WNDCLASSEX::lpfnWndProc
    // and pass "this" as lpParam in CreateWindowEx() to modify Window object
    // from windowProc().
    // More info: https://elcharolin.wordpress.com/2015/01/24/wndproc-as-a-class-member-win32/
    virtual LRESULT CALLBACK windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam) = 0;
    
    virtual void registerWindowClass(HINSTANCE hInstance) = 0;
    virtual void createWindow(HINSTANCE hInstance) = 0;
    
    HWND handle;
    HDC hdc;
    
    uint32_t width;
    uint32_t height;

    uint32_t posX;
    uint32_t posY;
};
