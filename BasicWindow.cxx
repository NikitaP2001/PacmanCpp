#include <stdexcept>

#include "BasicWindow.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

BasicWindow::BasicWindow()
{    
    if (!regWindowClass())
        throw std::runtime_error("Register class failed");

    HINSTANCE hInst = HINST_THISCOMPONENT;
    DWORD dwSyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    m_hwnd = CreateWindow(m_kClassName, m_kWndTitle, dwSyle, CW_USEDEFAULT,
        CW_USEDEFAULT, 0, 0, NULL, NULL, hInst, this);
    if (m_hwnd == NULL)
        throw std::runtime_error("Window creation failed");    
}


void BasicWindow::show() noexcept
{
    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
}


HWND BasicWindow::windowHandle() const noexcept
{
    return m_hwnd;
}


BasicWindow::~BasicWindow()
{
    CloseWindow(m_hwnd);
}


bool BasicWindow::setClientSize(double width, double height) noexcept
{
    bool status = false;    
    float dpi = static_cast<float>(GetDpiForWindow(m_hwnd));
    int scaledWidth = static_cast<int>(ceil(width * dpi / m_kDips));
    int scaledHeight = static_cast<int>(ceil(height * dpi / m_kDips));  

    RECT wr = { 0, 0, scaledWidth, scaledHeight };    
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    
    if (SetWindowPos(m_hwnd, NULL, NULL, NULL, wr.right - wr.left, wr.bottom - wr.top, 
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE))
        status = true;    

    return status;
}


void BasicWindow::RunMessageLoop() noexcept
{
    MSG msg{};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


HRESULT BasicWindow::createWindowEvent(HWND hWnd, LPARAM lParam) noexcept
{
    LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
    BasicWindow* pWnd = reinterpret_cast<BasicWindow*>(pcs->lpCreateParams);

    SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

    return 1;
}


LRESULT CALLBACK BasicWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{            
    BasicWindow* pApp = reinterpret_cast<BasicWindow*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (message == WM_CREATE) {
        return pApp->createWindowEvent(hWnd, lParam);        
    } else if (pApp) {

        switch (message) {
        case WM_SIZE:
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            return pApp->OnResize(width, height);            
        }
        case WM_DISPLAYCHANGE:
        {
            InvalidateRect(hWnd, NULL, FALSE);
            return 0;
        }
        case WM_PAINT:
        {
            LRESULT result = pApp->OnRender();
            ValidateRect(hWnd, NULL);            
            return result;
        }
        case WM_KEYDOWN:
        {
            pApp->OnKeydown(static_cast<SHORT>(wParam));
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        }
    }    
    return static_cast<HRESULT>(DefWindowProc(hWnd, message, wParam, lParam));
}


ATOM BasicWindow::regWindowClass() noexcept
{
    WNDCLASS wClass{};
    ATOM result = 1;
    if (!GetClassInfo(HINST_THISCOMPONENT, m_kClassName, &wClass)) {
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

        result = RegisterClassEx(&wcex);
    }
    return result;
}

