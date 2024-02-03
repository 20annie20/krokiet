#include "KrokietParser.h"
#include "KrokietRenderer.h"
#include "KrokietWindow.h"

#include <Windows.h>
#include <tchar.h>

INT WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    MSG  msg;
    bool quit = false;
    int  exitCode = 0;

    KrokietWindow window(hInstance, nCmdShow);
    KrokietRenderer renderer;

    renderer.setOutputWindow(window.previewWindowHandle());

    while(!quit) {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) {
                quit = true;
                exitCode = msg.lParam;
                continue;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(!quit) {
            renderer.renderFrame();
        }
    }

    return exitCode;
}
