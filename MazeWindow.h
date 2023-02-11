#pragma once

#include <mutex>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "MazeModel.h"

using error_code = enum {
    unsucceed = -1,
    succeed = 0,
};

class MazeWindow {

public:

    MazeWindow() = default;

    error_code init() noexcept;
      
    void RunMessageLoop() noexcept;

    void setModel(MazeModel *model) noexcept;

    ~MazeWindow();

protected:    

    HRESULT OnRender() noexcept;

    void OnResize(UINT width, UINT height) noexcept;

private:

    HRESULT LoadResourceBitmap(PCWSTR resourceName, PCWSTR resourceType,
        UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

    ATOM regWindowClass() noexcept;

    HRESULT createWindowEvent(HWND hWnd, LPARAM lParam) noexcept;

    HRESULT sizeEvent(LPARAM lParam) noexcept;

    error_code initOnce() noexcept;

    error_code setWindowSize(double height, double width) noexcept;

    HRESULT CreateDeviceIndependentResources() noexcept;
    
    HRESULT CreateDeviceResources() noexcept;
    
    void DiscardDeviceResources() noexcept;    
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;    
    

private:

    HWND m_hwnd = nullptr;
    ID2D1Factory* m_pDirect2dFactory = nullptr;
    ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;    
    ID2D1SolidColorBrush* m_pBlueBrush = nullptr;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush = nullptr;
    ID2D1Bitmap* m_pBmpSprites = nullptr;

    std::unique_ptr<MazeModel> m_model;

    std::once_flag m_initFlag;

    error_code m_status = error_code::unsucceed;

    static constexpr double m_kDips = 96.f;

    double m_pxlWndWidth  = 640.f;
    double m_pxlWndHeight = 480.f;
    
    static constexpr int m_kPxCellSize = 20;
    
    static constexpr TCHAR* m_kClassName = TEXT("D2DMazeWindow");
    static constexpr TCHAR* m_kWndTitle = TEXT("The Maze");
};