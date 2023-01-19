// GameEngine.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GameEngine.h"
#include "Application.h"
#include <sstream>

HINSTANCE Engine::m_Instance{};
Engine* Engine::m_pEngine{ nullptr };
int Engine::m_Show{};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Engine::Initialize(hInstance, nCmdShow);

    int result = Engine::GetSingleton()->Run(new Application());

    delete Engine::GetSingleton();

    return result;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return Engine::GetSingleton()->HandleEvent(hWnd, message, wParam, lParam);
}

Engine::Engine() {

    m_AppName = m_AppName = L"L-System Example - Stochastic Grammar";

    // Window class registration
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_Instance;
    wcex.hIcon = LoadIcon(m_Instance, MAKEINTRESOURCE(IDI_GAMEENGINE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = m_AppName.c_str();
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);
}

void Engine::Initialize(HINSTANCE hInstance, int nCmdShow) {
    m_Instance = hInstance;
    m_Show = nCmdShow;
}

int Engine::Run(Application* app) {
    m_pApplication = app;

    HWND hWnd = CreateWindowW(  m_AppName.c_str(),
                                m_AppName.c_str(),
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                CW_USEDEFAULT,
                                nullptr, 
                                nullptr, 
                                m_Instance, 
                                nullptr );

    if (!hWnd)
    {
        MessageBox(NULL, L"Failure to create program window", L"Error", MB_ICONERROR);
    }

    RECT rect;
    if (GetWindowRect(hWnd, &rect))
    {
        m_Width = float(rect.right - rect.left);
        m_Height = float(rect.bottom - rect.top);
    }

    ShowWindow(hWnd, m_Show);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(m_Instance, MAKEINTRESOURCE(IDC_GAMEENGINE));

    MSG msg;

    m_IsRunning = true;
    while (m_IsRunning)
    {
        if (PeekMessage(&msg, NULL,0,0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            auto start{ std::chrono::high_resolution_clock::now() };
            m_ElapsedSec = std::chrono::duration<float>(start - m_CurrentTime).count();
            m_CurrentTime = start;
            
            RECT rect;
            HDC hDC = GetDC(hWnd);
            GetClientRect(hWnd, &rect);

            // Double buffering code
            HDC hBufferDC = CreateCompatibleDC(hDC);
            // Create the buffer
            HBITMAP hBufferBmp = CreateCompatibleBitmap(hDC, int(m_Width), int(m_Height));
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hBufferDC, hBufferBmp);

            m_DrawDC = hBufferDC;
           
            m_pApplication->Update(m_ElapsedSec);
            m_pApplication->Paint();

            // As a last step copy the memdc to the hdc
            BitBlt(hDC, 0, 0, int(m_Width), int(m_Height), hBufferDC, 0, 0, SRCCOPY);

            // Reset the old bmp of the buffer, mainly for show since we kill it anyway
            SelectObject(hBufferDC, hOldBmp);
            // Kill the buffer
            DeleteObject(hBufferBmp);
            DeleteDC(hBufferDC);

            // Release HDC
            ReleaseDC(hWnd, hDC);
        }
    }

    return (int)msg.wParam;
}

LRESULT Engine::HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;

                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

        case WM_DESTROY:
            m_IsRunning = false;
            delete m_pApplication;

            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);

    }

    return 0;
}

Engine* Engine::GetSingleton() {
    if (!m_pEngine) {
        m_pEngine = new Engine();
    }

    return m_pEngine;
}

Engine::~Engine() {

}

float Engine::GetWidth() {
    return m_Width;
}

float Engine::GetHeight() {
    return m_Height;
}

void Engine::DrawLine(const Vector2D& start, const Vector2D& end) {

    HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // create Pen 
    HPEN oldPen = (HPEN)SelectObject(m_DrawDC, blackPen);

    MoveToEx(m_DrawDC, int(start.x), int(start.y), 0);
    LineTo(m_DrawDC, int(end.x), int(end.y));

    SelectObject(m_DrawDC, oldPen);   // put old Pen back 
    DeleteObject(blackPen);     // delete black Pen 
}

void Engine::ClearBackground() {
    HBRUSH hOldBrush, hNewBrush = CreateSolidBrush(RGB(255,255,255));
    HPEN hOldPen, hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

    hOldBrush = (HBRUSH)SelectObject(m_DrawDC, hNewBrush);
    hOldPen = (HPEN)SelectObject(m_DrawDC, hNewPen);

    Rectangle(m_DrawDC, 0, 0, int(m_Width), int(m_Height));

    SelectObject(m_DrawDC, hOldPen);
    SelectObject(m_DrawDC, hOldBrush);

    DeleteObject(hNewPen);
    DeleteObject(hNewBrush);
}

void Engine::DrawString(const std::string& text, RECT position) {

    std::wstringstream stream;
    stream << text.c_str();
    DrawText(m_DrawDC, stream.str().c_str(), -1, &position, DT_TOP);
}