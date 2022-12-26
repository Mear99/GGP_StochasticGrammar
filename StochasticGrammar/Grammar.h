#pragma once
#include <string>
#include "Nodes.h"

class Grammar
{
	public:
		Grammar( Node* rootNode);
		virtual ~Grammar() = default;

		Grammar(const Grammar&) = delete;
		Grammar(Grammar&&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		Grammar& operator=(Grammar&&) = delete;

		std::string GenerateSequence();

	private:
		Node* m_RootNode;
};

