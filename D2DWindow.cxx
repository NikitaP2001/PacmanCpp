#include <stdexcept>

#include "D2DWindow.h"

D2DWindow::D2DWindow()
{
    HRESULT hr = 0;
    hr = CreateDeviceIndependentResources();
    if (!SUCCEEDED(hr))    
        throw std::runtime_error("Create d2d resource failed");    
}


ID2D1Factory* D2DWindow::factory() const noexcept
{
    return m_mainFactory.Get();
}


ID2D1HwndRenderTarget* D2DWindow::renderTarget() const noexcept
{
    return m_renderTarget.Get();
}


HRESULT D2DWindow::LoadResourceBitmap(LPCTSTR resourceName, ID2D1Bitmap** ppBitmap) noexcept
{
    IWICImagingFactory* pIWICFactory = nullptr;
    const WICBitmapAlphaChannelOption option = WICBitmapUsePremultipliedAlpha;
    IWICBitmap* pIwicBmp = nullptr;
    HRESULT hr = E_FAIL;

    HBITMAP hBmp = LoadBitmap(GetModuleHandle(nullptr), resourceName);
    if (hBmp != nullptr) {
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory));
        Microsoft::WRL::ComPtr<IWICImagingFactory> iwicFactory(pIWICFactory);
        if (hr == S_OK) {
            hr = pIWICFactory->CreateBitmapFromHBITMAP(hBmp, nullptr, option, &pIwicBmp);
            Microsoft::WRL::ComPtr<IWICBitmap> iwicBitmap(pIwicBmp);
            if (hr == S_OK)
                hr = m_renderTarget->CreateBitmapFromWicBitmap(pIwicBmp, ppBitmap);
        }
        DeleteObject(hBmp);
    }
    return hr;
}


void D2DWindow::OnDraw(ID2D1HwndRenderTarget* pRenderTarget) noexcept
{

}


HRESULT D2DWindow::CreateDeviceResources() noexcept
{
    HRESULT hr = S_OK;
    ID2D1HwndRenderTarget* pTarget = nullptr;       
    RECT rc{};

    if (m_renderTarget.Get() == nullptr) {        

        GetClientRect(windowHandle(), &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        hr = m_mainFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(windowHandle(), size), &pTarget);
        m_renderTarget.Attach(pTarget);        
    }
    return hr;
}


void D2DWindow::DiscardDeviceResources() noexcept
{
    m_renderTarget.Reset();    
}


HRESULT D2DWindow::OnRender() noexcept
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr)) {
        m_renderTarget->BeginDraw();
        OnDraw(m_renderTarget.Get());
        hr = m_renderTarget->EndDraw();
    }    

    if (hr == D2DERR_RECREATE_TARGET) {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}


HRESULT D2DWindow::CreateDeviceIndependentResources() noexcept
{
    HRESULT hr = S_OK;
    ID2D1Factory *factory = nullptr;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (factory != nullptr)
        m_mainFactory.Attach(factory);
    
    return hr;
}