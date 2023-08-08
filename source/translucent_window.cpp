#include "translucent_window.h"

TranslucentWindow::TranslucentWindow(
    HINSTANCE hInstance,
    uint32_t width,
    uint32_t height,
    uint32_t posX,
    uint32_t posY) :
    IWindow(hInstance,
            width,
            height,
            posX,
            posY),
    pixels(nullptr)
{
    registerWindowClass(hInstance);
    createWindow(hInstance);
    
    initPixelsBuffer();
}

LRESULT CALLBACK TranslucentWindow::windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // DefWindowProc() calls WM_DESTROY when it gets WM_CLOSE, so
    // lets hide window in WM_CLOSE instead of destroy
    case WM_CLOSE:
    {
        ShowWindow(hWin, SW_HIDE);
        return 0;
    }    
    case WM_DESTROY:
    {
        // no need to exit
        break;
    }
    }
    
    return DefWindowProc(hWin, message, wParam, lParam);
}

void TranslucentWindow::registerWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX winDesc;
    ZeroMemory(&winDesc, sizeof(WNDCLASSEX));
    
    winDesc.cbSize = sizeof(WNDCLASSEX);
    winDesc.style = CS_HREDRAW | CS_VREDRAW;
    winDesc.lpfnWndProc = windowProc;
    winDesc.hInstance = hInstance;
    winDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winDesc.hbrBackground = 0; // transparent
    winDesc.lpszClassName = "WindowClassTranslucent";

    RegisterClassEx(&winDesc);
}

void TranslucentWindow::createWindow(HINSTANCE hInstance)
{
    hWin = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                          "WindowClassTranslucent",
                          "Translucent",
                          WS_POPUP,
                          posX,
                          posY,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    
    hdcWin = GetDC(hWin);
}

void TranslucentWindow::initBlendFunc()
{
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.BlendFlags = 0;
    blendFunc.AlphaFormat = AC_SRC_ALPHA; // source alpha (from bitmap/texture)
    blendFunc.SourceConstantAlpha = 0xFF; // constant alpha (100% for entire window)
}

void TranslucentWindow::initBitmapInfo()
{
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // ARGB 
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;
}

void TranslucentWindow::initPixelsBuffer()
{
    initBlendFunc();
    initBitmapInfo();
    
    hdcBitmap = CreateCompatibleDC(hdcWin);

    hBitmap = CreateDIBSection(hdcBitmap,
                               &bmi,
                               DIB_RGB_COLORS,
                               &pixels,
                               NULL,
                               0x0);

    SelectObject(hdcBitmap, hBitmap);

    // ARGB, should be premultiplied alpha (PMA)
    unsigned char red = 0xFF;
    unsigned char green = 0x00;
    unsigned char blue = 0x00;
    unsigned char alpha = 0x32; // 50%
    float alphaFactor = (float)alpha / (float)0xFF;
    
    for (uint32_t i = 0; i != width * height; ++i)
    {
        static_cast<uint32_t *>(pixels)[i] = (alpha << 24) |
                                             (static_cast<unsigned char>(red * alphaFactor) << 16) |
                                             (static_cast<unsigned char>(green * alphaFactor) << 8) |
                                             (static_cast<unsigned char>(blue * alphaFactor));
    }
}

void TranslucentWindow::setTexture(const Texture * texture)
{
    // TEST
    uint32_t winWidth = texture->getWidth();
    uint32_t winHeight = texture->getHeight();

    width = winWidth;
    height = winHeight;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    SetWindowPos(hWin,
                 HWND_TOPMOST,
                 (screenWidth - winWidth) / 2, // center of the screen
                 (screenHeight - winHeight) / 2,
                 winWidth,
                 winHeight,
                 SWP_SHOWWINDOW);

    bmi.bmiHeader.biWidth = winWidth;
    bmi.bmiHeader.biHeight = -winHeight;
    bmi.bmiHeader.biSizeImage = winWidth * winHeight * 4;

    hBitmap = CreateDIBSection(hdcBitmap,
                               &bmi,
                               DIB_RGB_COLORS,
                               &pixels,
                               NULL,
                               0x0);

    SelectObject(hdcBitmap, hBitmap);

    // TEST
    
    const unsigned char * data = texture->getData();
    const uint32_t size = texture->getSize();
    
    for (uint32_t i = 0, j = 0;
         i != size;
         i += 4, ++j)
    {
        unsigned char red = data[i];
        unsigned char green = data[i + 1];
        unsigned char blue = data[i + 2];
        unsigned char alpha = data[i + 3];
        float alphaFactor = (float)alpha / (float)0xFF;
        
        static_cast<uint32_t *>(pixels)[j] = (alpha << 24) |
                                             (static_cast<unsigned char>(red * alphaFactor) << 16) |
                                             (static_cast<unsigned char>(green * alphaFactor) << 8) |
                                             (static_cast<unsigned char>(blue * alphaFactor));
    }
}

void TranslucentWindow::Draw()
{    
    RECT coord;    
    GetWindowRect(hWin, &coord);
    
    POINT windowPosition = { coord.left, coord.top };
    SIZE windowSize = { static_cast<LONG>(width), static_cast<LONG>(height) };
    POINT imagePosition = { 0, 0 };
    
    UpdateLayeredWindow(hWin,
                        hdcWin,
                        &windowPosition,
                        &windowSize,
                        hdcBitmap,
                        &imagePosition,
                        0,
                        &blendFunc,
                        ULW_ALPHA);
}

TranslucentWindow::~TranslucentWindow()
{
    DeleteObject(hBitmap);
    DeleteDC(hdcWin);
}
