#pragma once
#include <d2d1_1.h>
#include <d3d11.h>

class KrokietRenderer {
    IDXGIFactory        *m_pdxgiFactory;
    ID3D11Device        *m_pd3dDevice;
    ID3D11DeviceContext *m_pd3dContext;
    ID2D1Factory1       *m_pd2dFactory;
    ID2D1Device         *m_pd2dDevice;
    ID2D1DeviceContext  *m_pd2dContext;

    HWND                    m_hWnd;
    IDXGISwapChain         *m_pdxgiSwapchain;
    ID3D11Texture2D        *m_pd3dSwapchainBackbuffer;
    ID3D11RenderTargetView *m_pd3dSwapchainRenderTarget;
    ID2D1Bitmap1           *m_pd2dSwapchainBitmap;

    ID2D1SolidColorBrush *m_pd2dBlackBrush;

public:
    KrokietRenderer();
    ~KrokietRenderer();

    HRESULT setOutputWindow(HWND hWnd);
    HWND    outputWindow() const;

    void renderFrame();

private:
    void releaseResources();
};
