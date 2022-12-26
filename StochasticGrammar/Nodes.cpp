#include "Nodes.h"

LeafNode::LeafNode(std::string value)
	: m_Value{ value }
{}

std::string LeafNode::Parse() {
	return m_Value;
}