#include "Application.h"
#include "GameEngine.h"
#include "Math.h"
#include <iostream>
#include <sstream>

#define ENGINE Engine::GetSingleton()

Application::Application() 
	: m_Fern{ new Grammar<std::function<void()>>}
{

	m_Fern->AddLeaveNode("F", [&]() {
		endPos.x = currentPos.x + segmentLength * cosf(currentAngle);
		endPos.y = currentPos.y - segmentLength * sinf(currentAngle);

		ENGINE->DrawLine(currentPos, endPos, riverWidth);
		currentPos = endPos;
	});

	m_Fern->AddLeaveNode("-", [&]() {
		currentAngle -= angle;
	});

	m_Fern->AddLeaveNode("+", [&]() {
		currentAngle += angle;
	});

	m_Fern->AddLeaveNode("[", [&]() {
		savedPos.push_back(currentPos);
		savedAngle.push_back(currentAngle);
		savedWidth.push_back(riverWidth);
	});

	m_Fern->AddLeaveNode("]", [&]() {
		currentPos = *(savedPos.end() - 1);
		currentAngle = *(savedAngle.end() - 1);
		riverWidth = *(savedWidth.end() - 1);
		savedPos.pop_back();
		savedAngle.pop_back();
		savedWidth.pop_back();
	});

	m_Fern->AddLeaveNode(">", [&]() {
		riverWidth *= 0.95f;
	});

	m_Fern->AddLeaveNode("X", [&]() {});

	m_Fern->ParseRule("Forward", "F -> F # 0.4 & >");
	m_Fern->ParseRule("Right", "F -> - & Forward");
	m_Fern->ParseRule("Left", "F -> + & Forward");
	m_Fern->ParseRule("TurnRight", "Right # 0.4");
	m_Fern->ParseRule("TurnLeft", "Left # 0.4");
	m_Fern->ParseRule("Fork", "F -> [ & > # 0.2 & X # 0.7 & ]");
	m_Fern->ParseRule("Segment", "F -> 0.4 Forward | 0.6 TurnRight | 0.6 TurnLeft | 0.2 Fork");

	m_Fern->ParseRule("X", "X -> X & Segment # 0.7");

	LNode< std::function<void()>>::SetDepth(10);
}

Application::~Application() {

}

void Application::Update(float elapsedSec) {
	m_DepthTimer += elapsedSec;
	if (m_DepthTimer > m_MaxDepthTimer) {
		m_DepthTimer -= m_MaxDepthTimer;

		// Generate new action vector
		result = m_Fern->GenerateSequence("X");
	}
}

void Application::Paint() {

		ENGINE->ClearBackground();

		// Set variables
		savedPos.clear();
		savedAngle.clear();
		segmentLength = 10.0f;
		currentAngle = 0;
		riverWidth = 25.0f;
		currentPos = Vector2D{ 0, ENGINE->GetHeight()/2 };
		endPos = Vector2D{ 0,0 };
	
	
		for (auto action : result) {
			action();
		}

		ENGINE->DrawString("Depth: " + std::to_string(m_Depth));
}