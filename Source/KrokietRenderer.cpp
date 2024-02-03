#include "KrokietRenderer.h"
#include <system_error>

KrokietRenderer::KrokietRenderer() try
      : m_pd3dDevice(NULL)
      , m_pd3dContext(NULL)
      , m_pd2dFactory(NULL)
      , m_pd2dDevice(NULL)
      , m_pd2dContext(NULL)
      , m_hWnd(NULL)
      , m_pdxgiSwapchain(NULL) {

    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pd3dDevice, NULL, &m_pd3dContext);
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

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

    D2D1_CREATION_PROPERTIES d2d1CreationProperties = {
        .threadingMode = D2D1_THREADING_MODE_SINGLE_THREADED};

    hr = D2D1CreateDevice(pdxgiDevice, d2d1CreationProperties, &m_pd2dDevice);
    pdxgiDevice->Release();
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
    if(FAILED(hr))
        throw std::system_error(hr, std::system_category());

} catch(...) {
    releaseResources();
    throw;
}

KrokietRenderer::~KrokietRenderer() {
    releaseResources();
}

void KrokietRenderer::releaseResources() {
    if(m_pdxgiSwapchain) m_pdxgiSwapchain->Release();
    if(m_pd2dContext) m_pd2dContext->Release();
    if(m_pd2dDevice) m_pd2dDevice->Release();
    if(m_pd2dFactory) m_pd2dFactory->Release();
    if(m_pd3dContext) m_pd3dContext->Release();
    if(m_pd3dDevice) m_pd3dDevice->Release();
    if(m_pdxgiFactory) m_pdxgiFactory->Release();
}

HRESULT KrokietRenderer::setOutputWindow(HWND hWnd) {
    HRESULT hr = S_OK;
    if(hWnd == m_hWnd)
        return hr;

    if(m_pdxgiSwapchain) {
        m_pdxgiSwapchain->Release();
        m_pdxgiSwapchain = NULL;
    }
    if(m_pd2dContext) {
        m_pd2dContext->Release();
        m_pd2dContext = NULL;
    }

    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc = {
        .OutputWindow = hWnd};

    hr = m_pdxgiFactory->CreateSwapChain(m_pd3dDevice, &dxgiSwapChainDesc, &m_pdxgiSwapchain);
    if(FAILED(hr))
        return hr;

    return hr;
}

HWND KrokietRenderer::outputWindow() const {
    return m_hWnd;
}
