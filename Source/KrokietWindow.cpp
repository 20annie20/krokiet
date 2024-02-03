#include "KrokietWindow.h"
#include <CommCtrl.h>
#include <system_error>
#include <tchar.h>

#define t TEXT

KrokietWindow::KrokietWindow(HINSTANCE hInstance, int nCmdShow)
      : m_hWnd(nullptr)
      , m_hInstance(hInstance) {

    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = wndproc,
        .hInstance = m_hInstance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH) (COLOR_BACKGROUND - 1),
        .lpszClassName = t("KrokietWindow")};

    if(!RegisterClass(&wc)) {
        DWORD err = GetLastError();
        if(err != ERROR_ALREADY_REGISTERED)
            throw std::system_error(err, std::system_category());
    }

    m_hWnd = CreateWindow(
        wc.lpszClassName, t("Krokiet"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480,
        NULL, NULL, m_hInstance, NULL);

    if(!m_hWnd) {
        throw std::system_error(GetLastError(), std::system_category());
    }

    HWND hApplyBtn = CreateWindow(
        WC_BUTTON, t("Apply"),
        WS_CHILD | WS_VISIBLE,
        10, 10, 160, 32, m_hWnd, (HMENU) 1, m_hInstance, NULL);

    HWND hTextBox = CreateWindow(
        WC_EDIT, t(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE,
        10, 47, 320, 100, m_hWnd, NULL, m_hInstance, NULL);

    m_hPreviewWnd = CreateWindow(
        WC_STATIC, t(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 152, 320, 200, m_hWnd, NULL, m_hInstance, NULL);

    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR) this);
    ShowWindow(m_hWnd, nCmdShow);
}

KrokietWindow::~KrokietWindow() {
    if(m_hWnd) {
        DestroyWindow(m_hWnd);
    }
}

HWND KrokietWindow::handle() const {
    return m_hWnd;
}

HWND KrokietWindow::previewWindowHandle() const {
    return m_hPreviewWnd;
}

LRESULT CALLBACK KrokietWindow::wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    KrokietWindow *wnd = (KrokietWindow *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if(wnd) {
        switch(msg) {
        case WM_DESTROY:
            wnd->m_hWnd = NULL;
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            if(LOWORD(wParam) == 1) {
                MessageBox(hWnd, t("Henlo"), t("Krokiet"), MB_OK);
                return 0;
            }
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
