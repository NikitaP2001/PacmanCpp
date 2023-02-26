#pragma once

#include <mutex>
#include <thread>

#include <Windows.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include "GameController.h"
#include "D2DWindow.h"


class MazeWindow : public D2DWindow {

public:

    MazeWindow();

    void setModel(GameModel *model) noexcept;    

    ~MazeWindow();

protected:

    void OnDraw(ID2D1HwndRenderTarget* pRenderTarget) noexcept override;

    HRESULT OnResize(UINT width, UINT height) noexcept override;

    HRESULT OnKeydown(SHORT vKeyCode) noexcept override;

    HRESULT CreateDeviceResources() noexcept override;

    void DiscardDeviceResources() noexcept override;

private:                        
    
    void redrawLoop() noexcept;

private:    

    Microsoft::WRL::ComPtr<ID2D1Bitmap> bmpSprites;

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blueBrush;

    std::thread m_redrawThread;
    bool m_continueRedraw = true;

    std::chrono::time_point<std::chrono::steady_clock> m_lastDraw{};

    static constexpr const int m_kMillisInSec = 1000;

    GameModel *m_model = nullptr;     

    double m_pxlWndWidth  = 640.f;
    double m_pxlWndHeight = 480.f;

    int m_framesPerSec = 48;
    
    static constexpr std::size_t m_kPxCellSize = 20;
    
    static constexpr const TCHAR* m_kClassName = TEXT("D2DMazeWindow");
    static constexpr const TCHAR* m_kWndTitle = TEXT("The Maze");
};