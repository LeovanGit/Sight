#include "window.h"

Window::Window(HINSTANCE hInstance,
               WNDPROC windowProc,
               uint32_t width,
               uint32_t height,
               uint32_t posX,
               uint32_t posY) :
               width(width),
               height(height),
               posX(posX),
               posY(posY)
{
    initWindowClass(hInstance, windowProc);

    hWin = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                          "WindowClass1",
                          "Title",
                          WS_POPUP,
                          posX,
                          posY,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    hdc = GetDC(hWin);

    ShowWindow(hWin, SW_SHOW);

    initPixelsBuffer();
}

void Window::initWindowClass(HINSTANCE hInstance, WNDPROC windowProc)
{
    WNDCLASSEX winDesc;
    ZeroMemory(&winDesc, sizeof(WNDCLASSEX));
    
    winDesc.cbSize = sizeof(WNDCLASSEX);
    winDesc.style = CS_HREDRAW | CS_VREDRAW;
    winDesc.lpfnWndProc = windowProc;
    winDesc.hInstance = hInstance;
    winDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winDesc.hbrBackground = 0; //(HBRUSH)COLOR_WINDOW;
    winDesc.lpszClassName = "WindowClass1";

    RegisterClassEx(&winDesc);
}

void Window::initPixelsBuffer()
{
    // TODO: do it in separate methods
    // BLENDFACTOR()
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.AlphaFormat = AC_SRC_ALPHA; // source alpha (from bitmap)
    bf.SourceConstantAlpha = 0xFF; // constant alpha (100% for entire window)

    hdcBitmap = CreateCompatibleDC(hdc);

    // BITMAPINFO()
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;

    // TODO: use std::vector later
    // BUFFER()
    // uint32_t size = width * height;
    // pixels.resize(size);
    // for (uint32_t pixel = 0; pixel != size; ++pixel)
    // {
    //     unsigned char alpha = 0x32;
    //     float alphaFactor = (float)alpha / (float)0xFF;
    //     // should be premultiplied
    //     unsigned char red = 0x00 * alphaFactor;
    //     unsigned char green = 0x00 * alphaFactor;
    //     unsigned char blue = 0x00 * alphaFactor;
        
    //     pixels[pixel] = alpha << 24 |
    //                     red << 16 |
    //                     green << 8 |
    //                     blue;
    // }

    hbitmap = CreateDIBSection(hdcBitmap,
                               &bmi,
                               DIB_RGB_COLORS,
                               &pvBits,
                               NULL,
                               0x0);

    unsigned char alpha = 0x32; // 50
    float alphaFactor = (float)alpha / (float)0xFF;
    // should be premultiplied
    unsigned char red = 0x00;
    unsigned char green = 0x00;
    unsigned char blue = 0x00;
    
    for (uint32_t y = 0; y != height; ++y)
        for (uint32_t x = 0; x != width; ++x)
        {
            ((UINT32 *)pvBits)[x + y * width] =
                (alpha << 24) |
                ((UCHAR)(red * alphaFactor) << 16) |
                ((UCHAR)(green * alphaFactor) << 8) |
                ((UCHAR)(blue * alphaFactor));
        }
}

void Window::setTexture(const Texture * texture)
{
    // uint32_t transparent = 0xC8C8C8;
    // uint32_t white = 0xFFFFFF;
    // uint32_t black = 0x000000;    
    
    // uint32_t size = texture->width * texture->height * texture->channels;
    // unsigned char * data = texture->data;

    // // + is wrong, should be | (look initpixelsbuffer())
    // uint32_t j = 0;
    // for (uint32_t i = 0; i != size; i += 3)
    // {
    //     uint32_t pixel = (data[i] << 16) + (data[i + 1] << 8) + data[i + 2];

    //     if (pixel == 0x0000FF) pixels[j] = transparent;
    //     else pixels[j] = pixel;
        
    //     ++j;
    // }

    unsigned char * data = texture->data;

    uint32_t j = 0;
    for (uint32_t i = 0; i != texture->width * texture->height * texture->channels; i += 4)
    {
        unsigned char red = data[i];
        unsigned char green = data[i + 1];
        unsigned char blue = data[i + 2];
        unsigned char alpha = data[i + 3];
        float alphaFactor = (float)alpha / (float)0xFF;
        
        ((UINT32 *)pvBits)[j] =
            (alpha << 24) |
            ((UCHAR)(red * alphaFactor) << 16) |
            ((UCHAR)(green * alphaFactor) << 8) |
            ((UCHAR)(blue * alphaFactor));

        ++j;
    }
}

void Window::Draw()
{
    SelectObject(hdcBitmap, hbitmap);
    
    RECT coord;
    GetWindowRect(hWin, &coord);
    POINT windowPosition = { coord.left, coord.top };
    SIZE windowSize = { static_cast<LONG>(width),
                        static_cast<LONG>(height) };
    POINT imagePosition = { 0, 0 };
    
    UpdateLayeredWindow(hWin,
                        hdc,
                        &windowPosition,
                        &windowSize,
                        hdcBitmap,
                        &imagePosition,
                        0,
                        &bf,
                        ULW_ALPHA);
}

Window::~Window()
{
    DeleteObject(hbitmap);
    DeleteDC(hdc);
}
