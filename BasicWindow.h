#pragma once

#include <Windows.h>
#include <wrl/wrappers/corewrappers.h>

class BasicWindow {

public:

    BasicWindow();

    _NODISCARD bool setWindowSize(double width, double height) noexcept;

    void RunMessageLoop() noexcept;

    void show() noexcept;

    HWND windowHandle() const noexcept;

    ~BasicWindow();

protected:

    virtual HRESULT OnRender() noexcept;

    virtual HRESULT OnResize(UINT width, UINT height) noexcept;

    virtual HRESULT OnKeydown(SHORT vKeyCode) noexcept;
    
private:

    HRESULT createWindowEvent(HWND hWnd, LPARAM lParam) noexcept;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

    ATOM regWindowClass() noexcept;    

private:

    HWND m_hwnd;  

    static constexpr double m_kDips = 96.f;

    static constexpr const TCHAR* m_kClassName = TEXT("BasicWindow");
    static constexpr const TCHAR* m_kWndTitle = TEXT("Window");

};