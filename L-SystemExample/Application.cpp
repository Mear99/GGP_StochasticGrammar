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

		ENGINE->DrawLine(currentPos, endPos);
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
	});

	m_Fern->AddLeaveNode("]", [&]() {
		currentPos = *(savedPos.end() - 1);
		currentAngle = *(savedAngle.end() - 1);
		savedPos.pop_back();
		savedAngle.pop_back();
	});

	m_Fern->AddLeaveNode("X", [&]() {});

	m_Fern->ParseRule("X", "X -> F & + & [ & [ & X & ] & - & X & ] & - & F & [ & - & F & X & ] & + & X");
	m_Fern->ParseRule("F", "F -> F & F");
}

Application::~Application() {

}

void Application::Update(float elapsedSec) {
	m_DepthTimer += elapsedSec;
	if (m_DepthTimer > m_MaxDepthTimer) {
		m_Depth++;
		m_Depth %= m_MaxDepth;
		m_DepthTimer -= m_MaxDepthTimer;
	}

}

void Application::Paint() {

		ENGINE->ClearBackground();

		// Generate action vector
		LNode< std::function<void()>>::SetDepth(m_Depth);
		result = m_Fern->GenerateSequence("X");

		// Set variables
		savedPos.clear();
		savedAngle.clear();
		segmentLength = ENGINE->GetHeight() * 0.37f / powf(2.0f, float(m_Depth));
		currentAngle = PI / 2;
		currentPos = Vector2D{ ENGINE->GetWidth() / 2, ENGINE->GetHeight() };
		endPos = Vector2D{ 0,0 };
	
	
		for (auto action : result) {
			action();
		}

		ENGINE->DrawString("Depth: " + std::to_string(m_Depth));
}