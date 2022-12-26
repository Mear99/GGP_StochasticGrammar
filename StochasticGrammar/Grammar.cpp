#include "Grammar.h"

Grammar::Grammar(Node* rootNode)
	: m_RootNode{ rootNode }
{}

std::string Grammar::GenerateSequence() {
	return m_RootNode->Parse();
}