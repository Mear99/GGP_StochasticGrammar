#include "Nodes.h"
#include <random>
#include <algorithm>

// Random float generator
std::random_device rd;

//
// Engines 
//
std::mt19937 e2(rd());


Node::Node() {};


LeafNode::LeafNode(std::string value)
	: m_Value{ value }
{}

std::string LeafNode::Parse() {
	return m_Value;
}

SelectNode::SelectNode() {};

std::string SelectNode::Parse() {

	if (m_pOptions.size() > 0) {
		int index{ WeightedRandom() };
		return m_pOptions[index].first->Parse();
	}
	return "";
}

void SelectNode::AddOption(Node* option, float weight) {
	m_pOptions.push_back(std::make_pair(option, weight));
	m_WeightsSum += weight;
}

int SelectNode::WeightedRandom() {
	std::uniform_real_distribution<> dist(0, m_WeightsSum);
	float randomWeight{ float(dist(e2)) };

	for (int index{ 0 }; index < m_pOptions.size(); ++index) {
		randomWeight -= m_pOptions[index].second;
		if (randomWeight < 0) {
			return index;
		}
	}
	return int(m_pOptions.size() - 1);
}

SequenceNode::SequenceNode() {};

std::string SequenceNode::Parse() {

	std::string result{};

	for (auto& element : m_pElements) {
		result += element->Parse();
		result += " ";
	}

	return result;
}

void SequenceNode::AddElement(Node* option) {
	m_pElements.push_back(option);
}

RepetitionNode::RepetitionNode(Node* node, int repetitions)
	: m_pNode{ node }
	, m_Repetitions{ repetitions }
{}

std::string RepetitionNode::Parse() {
	std::string result{};

	for (int i{ 0 }; i < m_Repetitions; ++i) {
		result += m_pNode->Parse();
		result += " ";
	}

	return result;
}