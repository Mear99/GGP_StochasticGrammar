#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <random>

//*** NODE ***
//
//

template<typename Data>
class Node {
	public:
		Node();
		virtual ~Node() = default;

		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;

		virtual void Parse(std::vector<Data>& result, int depth) = 0;
		virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) = 0;
};

template<typename Data>
Node<Data>::Node() {};

//*** LEAFNODE ***
//
//

template<typename Data>
class LeafNode : public Node<Data>
{
	public:
		LeafNode(Data value);
		virtual ~LeafNode() = default;

		LeafNode(const LeafNode&) = delete;
		LeafNode(LeafNode&&) = delete;
		LeafNode& operator=(const LeafNode&) = delete;
		LeafNode& operator=(LeafNode&&) = delete;

		virtual void Parse(std::vector<Data>& result, int depth) override;
		virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) override;

	private:
		Data m_Value;
};

template<typename Data>
LeafNode<Data>::LeafNode(Data value)
	: m_Value{ value }
{}

template<typename Data>
void LeafNode<Data>::Parse(std::vector<Data>& result, int depth) {

	result.push_back(m_Value);
}

template<typename Data>
void LeafNode<Data>::SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) {
	// No node dependencies
}

//*** SELECTNODE ***
//
//

template<typename Data>
class SelectNode : public Node<Data>
{
	public:
		SelectNode();
		virtual ~SelectNode() = default;

		SelectNode(const SelectNode&) = delete;
		SelectNode(SelectNode&&) = delete;
		SelectNode& operator=(const SelectNode&) = delete;
		SelectNode& operator=(SelectNode&&) = delete;

		virtual void Parse(std::vector<Data>& result, int depth) override;
		virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) override;
		void AddOption(Node<Data>* option, float weight);

	private:
		std::vector<std::pair<Node<Data>*, float>> m_pOptions;
		float m_WeightsSum{ 0 };

		int WeightedRandom();
};

template<typename Data>
SelectNode<Data>::SelectNode() {};

template<typename Data>
void SelectNode<Data>::Parse(std::vector<Data>& result, int depth) {

	if (m_pOptions.size() > 0) {
		int index{ WeightedRandom() };
		m_pOptions[index].first->Parse(result, depth);
	}
}

template<typename Data>
void SelectNode<Data>::SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) {
	for (auto& child : m_pOptions) {
		if (child.first == oldNode) {
			child.first = newNode;
		}
	}
}

template<typename Data>
void SelectNode<Data>::AddOption(Node<Data>* option, float weight) {
	m_pOptions.push_back(std::make_pair(option, weight));
	m_WeightsSum += weight;
}

template<typename Data>
int SelectNode<Data>::WeightedRandom() {
	float randomWeight{ float(((rand() % 100)/100.0f ) * int(m_WeightsSum)) };

	for (int index{ 0 }; index < m_pOptions.size(); ++index) {
		randomWeight -= m_pOptions[index].second;
		if (randomWeight < 0) {
			return index;
		}
	}
	return int(m_pOptions.size() - 1);
}

//*** SEQUENCENODE ***
//
//

template<typename Data>
class SequenceNode : public Node<Data>
{
public:
	SequenceNode();
	virtual ~SequenceNode() = default;

	SequenceNode(const SequenceNode&) = delete;
	SequenceNode(SequenceNode&&) = delete;
	SequenceNode& operator=(const SequenceNode&) = delete;
	SequenceNode& operator=(SequenceNode&&) = delete;

	virtual void Parse(std::vector<Data>& result, int depth) override;
	virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) override;
	void AddElement(Node<Data>* option);

private:
	std::vector<Node<Data>*> m_pElements;
};

template<typename Data>
SequenceNode<Data>::SequenceNode() {};

template<typename Data>
void SequenceNode<Data>::Parse(std::vector<Data>& result, int depth) {

	for (auto& element : m_pElements) {
		element->Parse(result, depth);
	}
}

template<typename Data>
void SequenceNode<Data>::SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) {
	for (auto& child : m_pElements) {
		if (child == oldNode) {
			child = newNode;
		}
	}
}

template<typename Data>
void SequenceNode<Data>::AddElement(Node<Data>* option) {
	m_pElements.push_back(option);
}

//*** REPETITIONNODE ***
//
//

template<typename Data>
class RepetitionNode : public Node<Data>
{
public:
	RepetitionNode(Node<Data>* node, float chance);
	virtual ~RepetitionNode() = default;

	RepetitionNode(const RepetitionNode&) = delete;
	RepetitionNode(RepetitionNode&&) = delete;
	RepetitionNode& operator=(const RepetitionNode&) = delete;
	RepetitionNode& operator=(RepetitionNode&&) = delete;

	virtual void Parse(std::vector<Data>& result, int depth) override;
	virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) override;

private:
	Node<Data>* m_pNode;
	float m_RepetitionChance{ 0.0f };
};

template<typename Data>
RepetitionNode<Data>::RepetitionNode(Node<Data>* node, float chance)
	: m_pNode{ node }
	, m_RepetitionChance{ chance }
{}

template<typename Data>
void RepetitionNode<Data>::Parse(std::vector<Data>& result, int depth) {

	do {
		m_pNode->Parse(result, depth);
	} 
	while (float((rand() % 100)/100.0f) <= m_RepetitionChance);
}

template<typename Data>
void RepetitionNode<Data>::SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) {
	if (m_pNode == oldNode) {
		m_pNode = newNode;
	}
}

//*** LNODE ***
//
//
template<typename Data>
class LNode : public Node<Data>
{
public:
	LNode(Node<Data>* node, std::shared_ptr<Node<Data>> fallback);
	virtual ~LNode() = default;

	LNode(const LNode&) = delete;
	LNode(LNode&&) = delete;
	LNode& operator=(const LNode&) = delete;
	LNode& operator=(LNode&&) = delete;

	virtual void Parse(std::vector<Data>& result, int depth) override;
	virtual void SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) override;
	static void SetDepth(int depth) { MaxDepth = depth; }

private:
	Node<Data>* m_pNode;
	std::shared_ptr<Node<Data>> m_pFallback;
	static int MaxDepth;
};

template<typename Data>
int LNode<Data>::MaxDepth{ 5 };

template<typename Data>
LNode<Data>::LNode(Node<Data>* node, std::shared_ptr<Node<Data>> fallback)
	: m_pNode{ node }
	, m_pFallback{ fallback }
{}

template<typename Data>
void LNode<Data>::Parse(std::vector<Data>& result, int depth) {

	if (depth >= MaxDepth) {
		m_pFallback->Parse(result, 0);
		return;
	}

	m_pNode->Parse(result, ++depth);
	--depth;

}

template<typename Data>
void LNode<Data>::SwapDependingNode(Node<Data>* oldNode, Node<Data>* newNode) {
	if (m_pNode == oldNode) {
		m_pNode = newNode;
	}
}