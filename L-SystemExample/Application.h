#pragma once

#include "resource.h"
#include <windows.h>
#include "Math.h"
#include <vector>
#include "Grammar.h"
#include <functional>

class Application final{

	public:
		Application();
		~Application();

		void Paint();
		void Update(float elapsedSec);

		Application(const Application& ref) = delete;
		Application(Application&& ref) = delete;
		Application& operator=(const Application& ref) = delete;
		Application& operator=(Application&& ref) = delete;

	private:
		float angle{ 25.0f * PI/180.0f };
		Grammar<std::function<void()>> * m_Fern;
		std::vector<std::function<void() >> result;

		int m_Depth{ 0 };
		int m_MaxDepth{ 9 };
		float m_DepthTimer{ 0.0f };
		float m_MaxDepthTimer{ 1.5f };

		std::vector<Vector2D> savedPos{};
		std::vector<float> savedAngle{ 0 };

		float segmentLength{ };
		float currentAngle{};
		Vector2D currentPos{0,0 };
		Vector2D endPos{ 0,0};
};