#include <Windows.h>

#include "MazeWindow.h"

template<class Interface>
static void SafeRelease(Interface** ppInterfaceToRelease) noexcept
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


error_code MazeWindow::init() noexcept
{
    try {
        std::call_once(m_initFlag, &MazeWindow::initOnce, this);
    }
    catch (const std::system_error&)
    {
        m_status = error_code::unsucceed;
    }    

    return m_status;
}


ATOM MazeWindow::regWindowClass() noexcept
{    
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LONG_PTR);
    wcex.hInstance = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wcex.lpszClassName = m_kClassName;

    return RegisterClassEx(&wcex);
}


error_code MazeWindow::initOnce() noexcept
{
    HRESULT hr{};
    error_code status = error_code::unsucceed;

    /* d2d resource initialization*/
    hr = CreateDeviceIndependentResources();
    if (SUCCEEDED(hr))
    {
        if (regWindowClass())
        {
            /* create window hidden to obtain dpi value and scale it */
            DWORD dwSyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            m_hwnd = CreateWindow(m_kClassName, m_kWndTitle, dwSyle, CW_USEDEFAULT,
                CW_USEDEFAULT, 0, 0, NULL, NULL, HINST_THISCOMPONENT, this);

            status = setWindowSize(m_pxlWndWidth, m_pxlWndHeight);

            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }        
    }

    return status;
}


HRESULT MazeWindow::createWindowEvent(HWND hWnd, LPARAM lParam) noexcept
{
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    MazeWindow* pWnd = reinterpret_cast<MazeWindow*>(pcs->lpCreateParams);

    SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

    return 1;
}


HRESULT MazeWindow::sizeEvent(LPARAM lParam) noexcept
{
    UINT width = LOWORD(lParam);
    UINT height = HIWORD(lParam);
    this->OnResize(width, height);
    return 0;
}


LRESULT CALLBACK MazeWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    bool wasHandled = false;
    HRESULT result = 0;

    MazeWindow* pApp = reinterpret_cast<MazeWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));       
    
    if (message == WM_CREATE)
    {
        result = pApp->createWindowEvent(hWnd, lParam);
        wasHandled = true;
    } else if (pApp) {

        switch (message) {            
            case WM_SIZE:
            {
                result = pApp->sizeEvent(lParam);
                wasHandled = true;
                break;
            }
            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hWnd, NULL, FALSE);
                result = 0;
                wasHandled = true;
                break;
            }
            case WM_PAINT:
            {
                pApp->OnRender();
                ValidateRect(hWnd, NULL);
                result = 0;
                wasHandled = true;
                break;
            }
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                result = 1;
                wasHandled = true;
                break;
            }
        }
    }

    if (!wasHandled)
    {
        result = static_cast<HRESULT>(DefWindowProc(hWnd, message, wParam, lParam));
    }    

    return result;
}


error_code MazeWindow::setWindowSize(double width, double height) noexcept
{
    error_code status = error_code::unsucceed;
    if (m_hwnd) {
        float dpi = static_cast<float>(GetDpiForWindow(m_hwnd));
        int scaledWidth = static_cast<int>(ceil(width * dpi / m_kDips));
        int scaledHeight = static_cast<int>(ceil(height * dpi / m_kDips));

        if (SetWindowPos(m_hwnd, NULL, NULL, NULL, scaledWidth, scaledHeight, SWP_NOMOVE))
            status = error_code::succeed;
    }

    return status;
}


HRESULT MazeWindow::CreateDeviceIndependentResources() noexcept
{    
    HRESULT hr = S_OK;
    
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    
    return hr;
}


void MazeWindow::DiscardDeviceResources() noexcept
{    
    SafeRelease(&m_pBlueBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
    SafeRelease(&m_pBmpSprites);
    SafeRelease(&m_pRenderTarget);
}


HRESULT MazeWindow::LoadResourceBitmap(PCWSTR resourceName, PCWSTR resourceType,
    UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICStream* pStream = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICImagingFactory* pIWICFactory = nullptr;
    IWICBitmapScaler* pScaler = NULL;

    HRSRC imageResHandle = NULL;
    HGLOBAL imageResDataHandle = NULL;
    void* pImageFile = NULL;
    DWORD imageFileSize = 0;   

    // Locate the resource.
    imageResHandle = FindResourceW(HINST_THISCOMPONENT, resourceName, resourceType);
    HRESULT hr = imageResHandle ? S_OK : E_FAIL;

    hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pIWICFactory));

    if (SUCCEEDED(hr) && imageResHandle)
    {        
        imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);

        hr = imageResDataHandle ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr) && imageResDataHandle)
    {        
        pImageFile = LockResource(imageResDataHandle);
        hr = pImageFile ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr) && pImageFile && imageResHandle)
    {        
        imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);
        hr = imageFileSize ? S_OK : E_FAIL;
    }

    if (SUCCEEDED(hr))    
        hr = pIWICFactory->CreateStream(&pStream);    

    if (SUCCEEDED(hr))            
        hr = pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);    

    if (SUCCEEDED(hr))            
        hr = pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);    

    if (SUCCEEDED(hr))    
        hr = pDecoder->GetFrame(0, &pSource);    

    if (SUCCEEDED(hr))            
        hr = pIWICFactory->CreateFormatConverter(&pConverter);    

    if (SUCCEEDED(hr))  
        hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 
            NULL, 0.f, WICBitmapPaletteTypeMedianCut);    

    if (SUCCEEDED(hr))    
        hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);

    SafeRelease(&pDecoder);
    SafeRelease(&pIWICFactory);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

    return hr;
}



HRESULT MazeWindow::CreateDeviceResources() noexcept
{
    HRESULT hr = S_OK;
    RECT rc{};

    if (!m_pRenderTarget)
    {        
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        /* Create a Direct2D render target. */
        hr = m_pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size), &m_pRenderTarget);

        if (SUCCEEDED(hr)) {
            /* Create a gray brush. */
            hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue),
                &m_pBlueBrush);
        }
        if (SUCCEEDED(hr)) {
            /* Create a blue brush. */
            hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), 
                &m_pCornflowerBlueBrush);
        }
        if (SUCCEEDED(hr))
            hr = LoadResourceBitmap(L"SpriteSheet", L"Image", 899, 746, &m_pBmpSprites);        
    }

    return hr;
}


void MazeWindow::OnResize(UINT width, UINT height) noexcept
{
    if (m_pRenderTarget)    
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));    
}

HRESULT MazeWindow::OnRender() noexcept
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();        
        int width = static_cast<int>(rtSize.width);
        int height = static_cast<int>(rtSize.height);        

        if (m_model) {           

            auto walls = m_model->getWalls(true);

            for (const auto& wall : walls) {
                m_pRenderTarget->DrawLine(
                    D2D1::Point2F(static_cast<FLOAT>(wall.xUL) * m_kPxCellSize, 
                        static_cast<FLOAT>(wall.yUL) * m_kPxCellSize),
                    D2D1::Point2F(static_cast<FLOAT>(wall.xRD) * m_kPxCellSize, 
                        static_cast<FLOAT>(wall.yRD) * m_kPxCellSize),
                    m_pBlueBrush, 3.f);
            }


        }       

        hr = m_pRenderTarget->EndDraw();
    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}


void MazeWindow::RunMessageLoop() noexcept
{
    MSG msg{};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


void MazeWindow::setModel(MazeModel* model) noexcept
{
    m_model.reset(model);
    std::size_t pxMapWidth = m_kPxCellSize * m_model->mapWidth();
    std::size_t pxMapHeight = m_kPxCellSize * m_model->mapHeight();
    setWindowSize(static_cast<double>(pxMapWidth), static_cast<double>(pxMapHeight));
}


MazeWindow::~MazeWindow()
{
    SafeRelease(&m_pDirect2dFactory);
    DiscardDeviceResources();
    if (m_hwnd)
        CloseWindow(m_hwnd);
}