#include "KrokietRenderer.h"
#include <system_error>

template<typename T>
    requires std::is_base_of_v<IUnknown, T>
static void releaseCom(T *&ptr) {
    if(ptr) {
        ptr->Release();
        ptr = NULL;
    }
}

KrokietRenderer::KrokietRenderer() try
      : m_pd3dDevice(NULL)
      , m_pd3dContext(NULL)
      , m_pd2dFactory(NULL)
      , m_pd2dDevice(NULL)
      , m_pd2dContext(NULL)
      , m_hWnd(NULL)
      , m_pdxgiSwapchain(NULL)
      , m_pd3dSwapchainBackbuffer(NULL)
      , m_pd3dSwapchainRenderTarget(NULL)
      , m_pd2dSwapchainBitmap(NULL) {

    // Create D3D11 device & context.
    HRESULT hr = D3D11CreateDevice(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
        NULL, 0,
        D3D11_SDK_VERSION,
        &m_pd3dDevice, NULL, &m_pd3dContext);
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

    // Get DXGI factory used to create the device.
    IDXGIDevice *pdxgiDevice = NULL;
    hr = m_pd3dDevice->QueryInterface(&pdxgiDevice);
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

    IDXGIAdapter *pdxgiAdapter = NULL;
    hr = pdxgiDevice->GetAdapter(&pdxgiAdapter);
    if(FAILED(hr)) {
        pdxgiDevice->Release();
        throw std::system_error(hr, std::system_category());
    }

    hr = pdxgiAdapter->GetParent(IID_PPV_ARGS(&m_pdxgiFactory));
    pdxgiAdapter->Release();
    if(FAILED(hr)) {
        pdxgiDevice->Release();
        throw std::system_error(hr, std::system_category());
    }

    // Create D2D1 factory, device & context.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    if(FAILED(hr)) {
        pdxgiDevice->Release();
        throw std::system_error(hr, std::system_category());
    }

    hr = m_pd2dFactory->CreateDevice(pdxgiDevice, &m_pd2dDevice);
    pdxgiDevice->Release();
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

    hr = m_pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pd2dContext);
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

    // Create common brushes.
    m_pd2dContext->CreateSolidColorBrush({1, 0, 0, 1}, &m_pd2dBlackBrush);

} catch(...) {
    releaseResources();
    throw;
}

KrokietRenderer::~KrokietRenderer() {
    releaseResources();
}

void KrokietRenderer::releaseResources() {
    releaseCom(m_pd2dBlackBrush);
    releaseCom(m_pd2dSwapchainBitmap);
    releaseCom(m_pd3dSwapchainRenderTarget);
    releaseCom(m_pd3dSwapchainBackbuffer);
    releaseCom(m_pdxgiSwapchain);
    releaseCom(m_pd2dContext);
    releaseCom(m_pd2dDevice);
    releaseCom(m_pd2dFactory);
    releaseCom(m_pd3dContext);
    releaseCom(m_pd3dDevice);
    releaseCom(m_pdxgiFactory);
}

HRESULT KrokietRenderer::setOutputWindow(HWND hWnd) {
    HRESULT hr = S_OK;
    if(hWnd == m_hWnd)
        return hr;

    releaseCom(m_pd2dSwapchainBitmap);
    releaseCom(m_pd3dSwapchainRenderTarget);
    releaseCom(m_pd3dSwapchainBackbuffer);
    releaseCom(m_pdxgiSwapchain);

    m_hWnd = hWnd;
    if(!m_hWnd)
        return hr;

    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc = {
        .BufferDesc = {
            .Format = DXGI_FORMAT_B8G8R8A8_UNORM},
        .SampleDesc = {1, 0},
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .OutputWindow = hWnd,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL};

    hr = m_pdxgiFactory->CreateSwapChain(m_pd3dDevice, &dxgiSwapChainDesc, &m_pdxgiSwapchain);
    if(FAILED(hr))
        return hr;

    hr = m_pdxgiSwapchain->GetBuffer(0, IID_PPV_ARGS(&m_pd3dSwapchainBackbuffer));
    if(FAILED(hr))
        return hr;

    D2D1_BITMAP_PROPERTIES1 d2dBitmapProperties = {
        .pixelFormat = {
            .format = dxgiSwapChainDesc.BufferDesc.Format,
            .alphaMode = D2D1_ALPHA_MODE_IGNORE},
        .dpiX = 96,
        .dpiY = 96,
        .bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW};

    IDXGISurface *pdxgiSurface = NULL;
    hr = m_pd3dSwapchainBackbuffer->QueryInterface(&pdxgiSurface);
    if(FAILED(hr))
        return hr;

    hr = m_pd2dContext->CreateBitmapFromDxgiSurface(pdxgiSurface, d2dBitmapProperties, &m_pd2dSwapchainBitmap);
    if(FAILED(hr))
        return hr;

    m_pd2dContext->SetTarget(m_pd2dSwapchainBitmap);
    return hr;
}

HWND KrokietRenderer::outputWindow() const {
    return m_hWnd;
}

void KrokietRenderer::renderFrame() {
    if(!m_pdxgiSwapchain)
        return;

    m_pd2dContext->BeginDraw();
    m_pd2dContext->DrawLine({10, 10}, {50, 50}, m_pd2dBlackBrush);
    m_pd2dContext->EndDraw();
    m_pdxgiSwapchain->Present(1, 0);
}
