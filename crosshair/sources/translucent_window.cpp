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
        showWindow(false);
        
        return 0;
    }
    case WM_SIZE:
    {
        uint32_t width = LOWORD(lParam);
        uint32_t height = HIWORD(lParam);
        onResize(width, height);
        
        return 0;
    }
    case WM_MOVE:
    {
        uint32_t posX = LOWORD(lParam);
        uint32_t posY = HIWORD(lParam);
        onMove(posX, posY);
        
        return 0;
    }
    }
    
    return DefWindowProc(hWin, message, wParam, lParam);
}

LRESULT CALLBACK TranslucentWindow::messageRouter(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    TranslucentWindow *win;

    if (message == WM_CREATE)
    {
        win = static_cast<TranslucentWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWin, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
    }
    else
    {
        win = reinterpret_cast<TranslucentWindow*>(GetWindowLongPtr(hWin, GWLP_USERDATA));
    }

    if (win)
        return win->windowProc(hWin, message, wParam, lParam);
    else
        // skip all messages before WM_CREATE
        // because on some Windows versions
        // WM_CREATE may not be the first msg
        // after CreateWindowEx() and
        // we will get garbage in win ptr
        return DefWindowProc(hWin, message, wParam, lParam);
}

void TranslucentWindow::registerWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX winDesc;
    ZeroMemory(&winDesc, sizeof(WNDCLASSEX));
    
    winDesc.cbSize = sizeof(WNDCLASSEX);
    winDesc.style = CS_HREDRAW | CS_VREDRAW;
    winDesc.lpfnWndProc = messageRouter;
    winDesc.hInstance = hInstance;
    winDesc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winDesc.hbrBackground = 0; // transparent
    winDesc.lpszClassName = L"WindowClassTranslucent";

    RegisterClassEx(&winDesc);
}

void TranslucentWindow::createWindow(HINSTANCE hInstance)
{
    handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
                            L"WindowClassTranslucent",
                            L"Translucent",
                            WS_POPUP,
                            posX,
                            posY,
                            width,
                            height,
                            NULL,
                            NULL,
                            hInstance,
                            this);
    
    hdc = GetDC(handle);
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
    bmi.bmiHeader.biHeight = -int32_t(height);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // ARGB 
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;

    hdcBitmap = CreateCompatibleDC(hdc);

    hBitmap = CreateDIBSection(hdcBitmap,
                               &bmi,
                               DIB_RGB_COLORS,
                               &pixels,
                               NULL,
                               0x0);

    SelectObject(hdcBitmap, hBitmap);
}

void TranslucentWindow::initPixelsBuffer()
{
    initBlendFunc();
    initBitmapInfo();
    
    // ARGB, should be premultiplied alpha (PMA)
    // DEFAULT CROSSHAIR
    unsigned char red = 0x00;
    unsigned char green = 0x00;
    unsigned char blue = 0x00;
    unsigned char alpha = 0x00; // 0%
    // unsigned char alpha = 0xB4; // 70% for chess pattern
    float alphaFactor = (float)alpha / (float)0xFF;

    // uint32_t block_width = 20; // in pixels

    uint32_t thickness = 2; // in pixels
    uint32_t min = (height - thickness) / 2;
    uint32_t max = (height + thickness) / 2;

    uint32_t length = 4; // in pixels
    
    // generate default texture:
    for (uint32_t i = 0; i != width * height; ++i)
    {
        // CHESS BOARD PATTERN:
        // unsigned char red = 0x54;
        // unsigned char green = 0x59;
        // unsigned char blue = 0x48;

        // uint32_t pixel_col = i % width;
        // uint32_t pixel_row = i / height;

        // uint32_t block_col = pixel_col / block_width;
        // uint32_t block_row = pixel_row / block_width;
        
        // if ((block_col + block_row) % 2 == 0)
        // {
        //     red = 0x74;
        //     green = 0x76;
        //     blue = 0x6A;
        // }

        uint32_t pixel_col = i % width;
        uint32_t pixel_row = i / height;
        
        if ((pixel_row >= min && pixel_row < max && pixel_col < length) || // left stick
            (pixel_row >= min && pixel_row < max && pixel_col >= width - length && pixel_col <= width) || // right
            (pixel_col >= min && pixel_col < max && pixel_row < length) || // top
            (pixel_col >= min && pixel_col < max && pixel_row >= height - length && pixel_col <= height)) // bot
        {
            // light green
            red = 0x2E;
            green = 0xFA;
            blue = 0x2E;
            alpha = 0xFF; // 100%
        }
        else
        {
            red = 0x00;
            green = 0x00;
            blue = 0x00;
            alpha = 0x00; // 0%
        }

        alphaFactor = (float)alpha / (float)0xFF;
        
        static_cast<uint32_t *>(pixels)[i] = (alpha << 24) |
                                             (static_cast<unsigned char>(red * alphaFactor) << 16) |
                                             (static_cast<unsigned char>(green * alphaFactor) << 8) |
                                             (static_cast<unsigned char>(blue * alphaFactor));
    }
}

void TranslucentWindow::setTexture(const Texture * texture)
{
    resize(texture->getWidth(), texture->getHeight());
    move((screenWidth - width) / 2, (screenHeight - height) / 2); // center of the screen
    
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

void TranslucentWindow::draw()
{
    RECT coord;
    GetWindowRect(handle, &coord);
    
    POINT windowPosition = { coord.left, coord.top };
    SIZE windowSize = { static_cast<LONG>(width), static_cast<LONG>(height) };
    POINT imagePosition = { 0, 0 };
    
    UpdateLayeredWindow(handle,
                        hdc,
                        &windowPosition,
                        &windowSize,
                        hdcBitmap,
                        &imagePosition,
                        0,
                        &blendFunc,
                        ULW_ALPHA);
}

void TranslucentWindow::onResize(uint32_t newWidth, uint32_t newHeight)
{
    width = newWidth;
    height = newHeight;
    
    bmi.bmiHeader.biWidth = newWidth;
    bmi.bmiHeader.biHeight = -int32_t(newHeight); // inverse by x
    bmi.bmiHeader.biSizeImage = newWidth * newHeight * 4;
    
    hBitmap = CreateDIBSection(hdcBitmap,
                               &bmi,
                               DIB_RGB_COLORS,
                               &pixels,
                               NULL,
                               0x0);
    
    SelectObject(hdcBitmap, hBitmap);
}

TranslucentWindow::~TranslucentWindow()
{
    DeleteObject(hBitmap);
    DeleteDC(hdc);
}
