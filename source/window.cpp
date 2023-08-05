#include "window.h"

IWindow::IWindow(HINSTANCE hInstance,
                 uint32_t width,
                 uint32_t height,
                 uint32_t posX,
                 uint32_t posY) :
                 width(width),
                 height(height),
                 posX(posX),
                 posY(posY)
{
    
}

