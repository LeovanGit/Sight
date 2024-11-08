#include "window.h"

IWindow::IWindow(HINSTANCE hInstance,
                 uint32_t width,
                 uint32_t height,
                 uint32_t posX,
                 uint32_t posY) :
                 width(width),
                 height(height),
                 posX(posX),
                 posY(posY),
                 isShowed(false)
{
    
}

IWindow::~IWindow() {};

HWND IWindow::getHandle() const { return handle; }

bool IWindow::getWindowVisibility() const
{
    return isShowed;
}

void IWindow::showWindow(bool show)
{
    if (show)
    {
        ShowWindow(handle, SW_SHOW);
        isShowed = true;
    }
    else
    {
        isShowed = false;
        ShowWindow(handle, SW_HIDE);
    }
}

void IWindow::resize(uint32_t newWidth, uint32_t newHeight)
{
    // should also call onResize(), because SetWindowPos() will not generate WM_SIZE
    // if new size is equal to current
    SetWindowPos(handle,
                 0,
                 0,
                 0,
                 newWidth,
                 newHeight,
                 SWP_NOMOVE | SWP_NOZORDER);

    onResize(newWidth, newHeight);
}

void IWindow::move(uint32_t newPosX, uint32_t newPosY)
{
    // should also call onMove(), because SetWindowPos() will not generate WM_MOVE
    // if new pos is equal to current
    SetWindowPos(handle,
                 0,
                 newPosX,
                 newPosY,
                 0,
                 0,
                 SWP_NOSIZE | SWP_NOZORDER);

    onMove(newPosX, newPosY);
}

void IWindow::onResize(uint32_t newWidth, uint32_t newHeight)
{
    width = newWidth;
    height = newHeight;
}

void IWindow::onMove(uint32_t newPosX, uint32_t newPosY)
{
    posX = newPosX;
    posY = newPosY;
}
