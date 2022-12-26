#pragma once
#include <string>

class LeafNode
{
	public:
		LeafNode(std::string value);
		virtual ~LeafNode() = default;

		LeafNode(const LeafNode&) = delete;
		LeafNode(LeafNode&&) = delete;
		LeafNode& operator=(const LeafNode&) = delete;
		LeafNode& operator=(LeafNode&&) = delete;

		std::string Parse();

	private:
		std::string m_Value;
};

