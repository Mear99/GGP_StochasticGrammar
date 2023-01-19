#pragma once

#include "resource.h"
#include "Math.h"
#include <string>
#include <chrono>
#include <ctime> 

class Application;

class Engine final{

	public:
		~Engine();

		Engine(const Engine& ref) = delete;
		Engine(Engine&& ref) = delete;
		Engine& operator=(const Engine& ref) = delete;
		Engine& operator=(Engine&& ref) = delete;

		static void Initialize(HINSTANCE hInstance, int nCmdShow);
		int Run(Application* app);
		LRESULT HandleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static Engine* GetSingleton();

		// API functions
		float GetWidth();
		float GetHeight();

		void DrawLine(const Vector2D& start, const Vector2D& end, float width = 1.0f);
		void ClearBackground();
		void DrawString(const std::string& text, RECT position = RECT{0,0,500,500});


	private:
		Engine(); // Private constructor
		static Engine* m_pEngine;
		std::wstring m_AppName;

		Application* m_pApplication;

		static HINSTANCE m_Instance;
		static int m_Show;
		HDC m_hDC;
		HDC m_DrawDC;

		float m_Width;
		float m_Height;
		const int FPS = 200;

		float m_ElapsedSec;
		std::chrono::steady_clock::time_point m_CurrentTime = std::chrono::high_resolution_clock::now();
		bool m_IsRunning;
};
