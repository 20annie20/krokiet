#pragma once
#include <Windows.h>
#include <windowsx.h>

class KrokietWindow {
    HWND      m_hWnd;
    HINSTANCE m_hInstance;

    HWND m_hPreviewWnd;

public:
    KrokietWindow(HINSTANCE hInstance, int nCmdShow);
    ~KrokietWindow();

    HWND handle() const;
    HWND previewWindowHandle() const;

private:
    static LRESULT CALLBACK wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
