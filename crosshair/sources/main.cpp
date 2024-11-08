#include <iostream>
#include <windows.h>
#include <memory>

#include "translucent_window.h"
#include "menu_window.h"
#include "texture.h"

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

// variable to store the HANDLE to the hook. Don't declare it anywhere else then globally
// or you will get problems since every function uses this variable.
HHOOK _hook;
 
// This struct contains the data received by the hook callback. As you see in the callback function
// it contains the thing you will need: vkCode = virtual key code.
KBDLLHOOKSTRUCT kbdStruct;

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

MenuWindow *mw = nullptr;

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
#ifdef LOGS
    std::cout << "Screen size: " << screenWidth << "x" << screenHeight << "\n";
#endif

    MenuWindow menuWin(hInstance,
                       400,
                       400,
                       10,
                       10);
    mw = &menuWin;

    menuWin.showWindow();

    // default crosshair
    menuWin.crosshairWin->draw();
    menuWin.crosshairWin->showWindow();

    // handle key even if window is minimized (not focused):
    _hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            // a key (non-system) is pressed.
            if (kbdStruct.vkCode == VK_OEM_PLUS)
            {
                mw->crosshairWin->showWindow(!mw->crosshairWin->getWindowVisibility());
                //MessageBox(NULL, "+ is pressed!", "key pressed", MB_ICONINFORMATION);
            }
        }
    }
 
    // call the next hook in the hook chain
    // This is nessecary or your hook chain will break and the hook stops
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}
