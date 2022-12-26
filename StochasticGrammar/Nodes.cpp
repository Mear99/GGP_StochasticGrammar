#include "Nodes.h"
#include <random>
#include <algorithm>

Node::Node() {};


LeafNode::LeafNode(std::string value)
	: m_Value{ value }
{}

std::string LeafNode::Parse() {
	return m_Value;
}

SelectNode::SelectNode() {};

std::string SelectNode::Parse() {

	std::random_device rd;
	std::shuffle(m_pOptions.begin(), m_pOptions.end(), rd);

	return m_pOptions[0]->Parse();
}

void SelectNode::AddOption(Node* option) {
	m_pOptions.push_back(option);
}