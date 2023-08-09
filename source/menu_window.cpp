#include "menu_window.h"

extern std::unique_ptr<MenuWindow> menuWin;

enum ButtonID
{
    APPLY = 0,
    RESET,
    HIDE,
};

MenuWindow::MenuWindow(HINSTANCE hInstance,
                       uint32_t width,
                       uint32_t height,
                       uint32_t posX,
                       uint32_t posY) :
                       IWindow(hInstance,
                               width,
                               height,
                               posX,
                               posY),
                       nextTexture(nullptr)
{
    adjustSize();
    registerWindowClass(hInstance);
    createWindow(hInstance);

    initCrosshair(hInstance);
    
    // should be after createWindow(), because it uses handle
    // and after initCrosshair(), because addImage()
    addContent();
}

LRESULT CALLBACK MenuWindow::windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // note that we should use hWin in WM_CREATE, because this msg will generated
    // before CreateWindow() return and set "handle" class field
    case WM_CREATE:
    {
        // moved to constructor:
        // addContent();
        
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ButtonID::APPLY:
        {
            crosshairWin->setTexture(nextTexture.get());
            crosshairWin->draw();
            
            return 0;
        }
        case ButtonID::RESET:
        {
            crosshairWin->reset();
            crosshairWin->draw();
            
            return 0;
        }
        case ButtonID::HIDE:
        {
            crosshairWin->showWindow(!IsWindowVisible(crosshairWin->getHandle()));
            
            return 0;
        }        
        }
        
        break;
    }
    case WM_DESTROY:
    {
        // if menu is closed then we should exit app
        PostQuitMessage(0);
        
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
    case WM_DROPFILES:
    {
        // handle only first file and ignore other (0)
        HDROP hDrop = reinterpret_cast<HDROP>(wParam);
        
        std::string filename;
        uint32_t size = DragQueryFileA(hDrop, 0, NULL, 0);
        if (size > 0)
        {
            filename.resize(size);
            DragQueryFileA(hDrop, 0, &filename[0], size + 1);

            nextTexture = std::make_unique<Texture>(filename);
            updatePixelsBuffer();
            drawImage();
        }
        
        DragFinish(hDrop);
        
        return 0;
    }
    }
    
    return DefWindowProc(hWin, message, wParam, lParam);
}

LRESULT CALLBACK MenuWindow::messageRouter(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam)
{
    MenuWindow *win;

    if (message == WM_CREATE)
    {
        win = static_cast<MenuWindow*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWin, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
    }
    else
    {
        win = reinterpret_cast<MenuWindow*>(GetWindowLongPtr(hWin, GWLP_USERDATA));
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

void MenuWindow::adjustSize()
{
    RECT clientAreaSize { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&clientAreaSize,
                     WS_OVERLAPPEDWINDOW,
                     false);
    
    width = clientAreaSize.right - clientAreaSize.left;
    height = clientAreaSize.bottom - clientAreaSize.top;
}

void MenuWindow::registerWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX desc;
    ZeroMemory(&desc, sizeof(WNDCLASSEX));
    
    desc.cbSize = sizeof(WNDCLASSEX);
    desc.style = CS_HREDRAW | CS_VREDRAW;
    desc.lpfnWndProc = messageRouter;
    desc.hInstance = hInstance;
    desc.hCursor = LoadCursor(NULL, IDC_ARROW);
    desc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    desc.lpszClassName = "WindowClassMenu";
    
    RegisterClassEx(&desc);
}

void MenuWindow::createWindow(HINSTANCE hInstance)
{
    handle = CreateWindowEx(WS_EX_ACCEPTFILES,
                            "WindowClassMenu",
                            "Menu",
                            WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, // forbid resizing
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

void MenuWindow::addContent()
{
    addButton(ButtonID::APPLY, "Apply", 100, 30, 120, 10);
    addButton(ButtonID::RESET, "Reset", 100, 30, 120, 45);
    addButton(ButtonID::HIDE, "Hide", 100, 30, 120, 80);

    // TODO:
    //addLabel("text text text", 100, 30, 10, 10);
    addImage(100, 100, 10, 10);
}

void MenuWindow::addButton(int id,
                           const std::string & text,
                           uint32_t width,
                           uint32_t height,
                           uint32_t posX,
                           uint32_t posY)
{
    HWND hButton = CreateWindowEx(0,
                                  "BUTTON", // predefined window class
                                  text.c_str(),
                                  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                  posX,
                                  posY,
                                  width,
                                  height,
                                  handle,
                                  reinterpret_cast<HMENU>(id),
                                  (HINSTANCE)GetWindowLongPtr(handle, GWLP_HINSTANCE),
                                  NULL);
}

void MenuWindow::addLabel(const std::string & text,
                          uint32_t width,
                          uint32_t height,
                          uint32_t posX,
                          uint32_t posY)
{
    HWND hLabel = CreateWindowEx(0,
                                 "STATIC", // predefined window class
                                 NULL,
                                 WS_VISIBLE | WS_CHILD,
                                 posX,
                                 posY,
                                 width,
                                 height,
                                 handle,
                                 NULL,
                                 (HINSTANCE)GetWindowLongPtr(handle, GWLP_HINSTANCE),
                                 NULL);
    
    SetWindowText(hLabel, "Text");
}

void MenuWindow::addImage(uint32_t width,
                          uint32_t height,
                          uint32_t posX,
                          uint32_t posY)
{
    hImage = CreateWindowEx(0,
                            "STATIC", // predefined window class
                            NULL,
                            WS_VISIBLE | WS_CHILD,
                            posX,
                            posY,
                            width,
                            height,
                            handle,
                            NULL,
                            (HINSTANCE)GetWindowLongPtr(handle, GWLP_HINSTANCE),
                            NULL);

    //SendMessage(hImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)crosshairWin->getBitmap());
    
    hdcImage = GetDC(hImage);

    // TODO: move this later to separate method
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // ARGB 
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;

    pixels.resize(width * height);

    for (uint32_t i = 0; i != pixels.size(); ++i) pixels[i] = 0xFFFFFF;
}

void MenuWindow::updatePixelsBuffer()
{
    if (!nextTexture) return;

    const unsigned char * data = nextTexture->getData();
    uint32_t size = nextTexture->getSize();
    uint32_t width = nextTexture->getWidth();
    uint32_t height = nextTexture->getHeight();

    uint32_t pixelsHeight = 100;
    uint32_t pixelsWidth = 100;
    // next texture size can be bigger than pixel buffer (100x100), so
    // we need to take only its center
    if (width * height > pixels.size())
    {
        for (uint32_t i = 0; i != pixels.size(); ++i)
        {
            uint32_t col = i % pixelsWidth;
            uint32_t row = i / pixelsHeight;

            uint32_t deltaWidth = (width - pixelsWidth) / 2;
            uint32_t deltaHeight = (height - pixelsHeight) / 2;
            
            uint32_t j = ((row + deltaHeight) * width + (col + deltaWidth)) * 4;
            unsigned char red = data[j];
            unsigned char green = data[j + 1];
            unsigned char blue = data[j + 2];
            unsigned char alpha = data[j + 3];

            if (alpha == 0x00)
                pixels[i] = 0xFFFFFF;
            else
                pixels[i] = red << 16 | green << 8 | blue;
        }
    }
    // next texture size can be smaller than pixel buffer (100x100), so
    // we need to place it at the center of pixel buffer
    else
    {
        // clear
        for (uint32_t i = 0; i != pixels.size(); ++i) pixels[i] = 0xFFFFFF;

        for (uint32_t i = 0; i != size; i += 4)
        {
            unsigned char red = data[i];
            unsigned char green = data[i + 1];
            unsigned char blue = data[i + 2];
            unsigned char alpha = data[i + 3];
            
            if (alpha == 0x00) continue;
            
            uint32_t col = (i / 4) % width;
            uint32_t row = (i / 4) / height;
            
            uint32_t deltaWidth = (pixelsWidth - width) / 2;
            uint32_t deltaHeight = (pixelsHeight - height) / 2;
            
            uint32_t j = ((row + deltaHeight) * pixelsWidth + (col + deltaWidth));
            
            pixels[j] = red << 16 | green << 8 | blue;
        }        
    }
}

void MenuWindow::drawImage()
{
    uint32_t pixelsHeight = 100;
    uint32_t pixelsWidth = 100;
    
    SetDIBitsToDevice(hdcImage,
                      0,
                      0,
                      pixelsWidth,
                      pixelsHeight,
                      0,
                      0,
                      0,
                      pixelsHeight,
                      pixels.data(),
                      &bmi,
                      DIB_RGB_COLORS);
}

void MenuWindow::initCrosshair(HINSTANCE hInstance)
{
    crosshairWin = std::make_unique<TranslucentWindow>(hInstance);
}

MenuWindow::~MenuWindow()
{
    DeleteDC(hdcImage);
    DeleteDC(hdc);
}
