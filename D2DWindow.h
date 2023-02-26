#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include "BasicWindow.h"

class D2DWindow : public BasicWindow {

public:

    D2DWindow();

    ID2D1Factory *factory() const noexcept;

    ID2D1HwndRenderTarget* renderTarget() const noexcept;

    HRESULT LoadResourceBitmap(LPCTSTR resourceName, ID2D1Bitmap** ppBitmap) noexcept;

protected:

    virtual void OnDraw(ID2D1HwndRenderTarget* pRenderTarget) noexcept;

    virtual HRESULT CreateDeviceResources() noexcept;

    virtual void DiscardDeviceResources() noexcept;

private:

    HRESULT OnRender() noexcept override;

    HRESULT CreateDeviceIndependentResources() noexcept;

private:

    Microsoft::WRL::ComPtr<ID2D1Factory> m_mainFactory;    
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
    
};