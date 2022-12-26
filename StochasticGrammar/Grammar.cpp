#include "Grammar.h"

Grammar::Grammar(LeafNode* rootNode)
	: m_RootNode{ rootNode }
{}

std::string Grammar::GenerateSequence() {
	return m_RootNode->Parse();
}