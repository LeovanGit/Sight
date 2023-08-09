#pragma once

#include <memory>

#include "window.h"
#include "translucent_window.h"

class MenuWindow : public IWindow
{
public:
    MenuWindow(HINSTANCE hInstance,
               uint32_t width,
               uint32_t height,
               uint32_t posX,
               uint32_t posY);
    
    // temporarily unavailable
    // deleted methods should be public for better error messages
    MenuWindow(MenuWindow &) = delete;
    MenuWindow(MenuWindow &&) = delete;
    void operator=(MenuWindow &) = delete;
    void operator=(MenuWindow &&) = delete;

    ~MenuWindow();

    std::unique_ptr<TranslucentWindow> crosshairWin;
    
protected:
    virtual LRESULT CALLBACK windowProc(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK messageRouter(HWND hWin, UINT message, WPARAM wParam, LPARAM lParam);

    void adjustSize();
    virtual void registerWindowClass(HINSTANCE hInstance);
    virtual void createWindow(HINSTANCE hInstance);

    void addContent();
    
    void addButton(int id,
                   const std::string & text,
                   uint32_t width,
                   uint32_t height,
                   uint32_t posX,
                   uint32_t posY);

    void addLabel(const std::string & text,
                  uint32_t width,
                  uint32_t height,
                  uint32_t posX,
                  uint32_t posY);

    void addImage(uint32_t width,
                  uint32_t height,
                  uint32_t posX,
                  uint32_t posY);
    
    void initCrosshair(HINSTANCE hInstance);
    
    std::unique_ptr<Texture> nextTexture;

    // TODO: move this later into separate class like ImageWindow
    void updatePixelsBuffer();
    void drawImage();
    HWND hImage;
    HDC hdcImage;
    BITMAPINFO bmi;
    std::vector<uint32_t> pixels;
};
