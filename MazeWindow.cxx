#include <chrono>

#include "MazeWindow.h"
#include "handle.hpp"

using namespace Cell;

MazeWindow::MazeWindow()
    : m_redrawThread(&MazeWindow::redrawLoop, this)
{    
    BasicWindow::show();
}


void MazeWindow::setModel(GameModel* model) noexcept
{
    m_model = model;
    MazeModel* maze = m_model->maze();
    std::size_t pxMapWidth = m_kPxCellSize * maze->mapSize().width;
    std::size_t pxMapHeight = m_kPxCellSize * maze->mapSize().height;
    setClientSize(static_cast<double>(pxMapWidth), static_cast<double>(pxMapHeight));        
}


MazeWindow::~MazeWindow()
{
    m_continueRedraw = false;
    m_redrawThread.join();
}


void MazeWindow::OnDraw(ID2D1HwndRenderTarget* pRenderTarget)
{

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    D2D1_SIZE_F rtSize = pRenderTarget->GetSize();
    int width = static_cast<int>(rtSize.width);
    int height = static_cast<int>(rtSize.height);

    if (m_model) {

        auto walls = m_model->maze()->getWalls(true);        

        auto entities = m_model->entities();
        for (const auto entity : entities) {
            D2D_RECT_F sheetSegment = entity->sprite();
            auto destUpperLeft = entity->position() * m_kPxCellSize;
            D2D_RECT_F destRect = { destUpperLeft.x, destUpperLeft.y,
                destUpperLeft.x + m_kPxCellSize, destUpperLeft.y + m_kPxCellSize };
            pRenderTarget->DrawBitmap(bmpSprites.Get(), destRect, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sheetSegment);
        }

        for (const auto& wall : walls) {
            auto&& rect = wall.rect;
            pRenderTarget->DrawLine(
                D2D1::Point2F(static_cast<FLOAT>(rect.left) * m_kPxCellSize,
                    static_cast<FLOAT>(rect.top) * m_kPxCellSize),
                D2D1::Point2F(static_cast<FLOAT>(rect.right) * m_kPxCellSize,
                    static_cast<FLOAT>(rect.bottom) * m_kPxCellSize),
                m_blueBrush.Get(), 4.f);
        }
    }
}


HRESULT MazeWindow::OnKeydown(SHORT vKeyCode)
{
    if (m_model)
        m_model->setPlayerKey(vKeyCode);
    return S_OK;
}


HRESULT MazeWindow::CreateDeviceResources() noexcept
{
    HRESULT hr = D2DWindow::CreateDeviceResources();
    ID2D1SolidColorBrush *pBlueBrush = nullptr;
    ID2D1Bitmap* pSprites = nullptr;
    if (SUCCEEDED(hr) && bmpSprites.Get() == nullptr) {
        hr = LoadResourceBitmap(L"SpriteSheet", &pSprites);
        bmpSprites.Attach(pSprites);

        if (SUCCEEDED(hr)) {
            hr = renderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue),
                &pBlueBrush);
            m_blueBrush.Attach(pBlueBrush);
        }
    }
    return hr;
}


void MazeWindow::DiscardDeviceResources() noexcept
{
    bmpSprites.Reset();
    m_blueBrush.Reset();
}


void MazeWindow::redrawLoop() noexcept
{    
    while (m_continueRedraw) {

        auto currTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - m_lastDraw);
        if (duration.count() > m_kMillisInSec / m_framesPerSec) {
            if (!GetUpdateRect(windowHandle(), nullptr, false)) {
                InvalidateRect(windowHandle(), NULL, false);
                UpdateWindow(windowHandle());
                m_lastDraw = std::chrono::steady_clock::now();
            }
        }                        
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }    
}