#pragma once
#include <vector>
#include <string>
#include <random>
#include <algorithm>

#define MAXDEPTH 5

// Random float generator
std::random_device rd;
std::mt19937 e2(rd());

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

	private:
		Data m_Value;
};

template<typename Data>
LeafNode<Data>::LeafNode(Data value)
	: m_Value{ value }
{}

template<typename Data>
void LeafNode<Data>::Parse(std::vector<Data>& result, int depth) {

	if (depth > MAXDEPTH) {
		return;
	}

	result.push_back(m_Value);
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

	if (depth > MAXDEPTH) {
		return;
	}

	if (m_pOptions.size() > 0) {
		int index{ WeightedRandom() };
		m_pOptions[index].first->Parse(result, ++depth);
	}
	--depth;
}

template<typename Data>
void SelectNode<Data>::AddOption(Node<Data>* option, float weight) {
	m_pOptions.push_back(std::make_pair(option, weight));
	m_WeightsSum += weight;
}

template<typename Data>
int SelectNode<Data>::WeightedRandom() {
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
	void AddElement(Node<Data>* option);

private:
	std::vector<Node<Data>*> m_pElements;
};

template<typename Data>
SequenceNode<Data>::SequenceNode() {};

template<typename Data>
void SequenceNode<Data>::Parse(std::vector<Data>& result, int depth) {

	if (depth > MAXDEPTH) {
		return;
	}

	for (auto& element : m_pElements) {
		++depth;
		element->Parse(result, depth);
		--depth;
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
	RepetitionNode(Node<Data>* node, int repetitions);
	virtual ~RepetitionNode() = default;

	RepetitionNode(const RepetitionNode&) = delete;
	RepetitionNode(RepetitionNode&&) = delete;
	RepetitionNode& operator=(const RepetitionNode&) = delete;
	RepetitionNode& operator=(RepetitionNode&&) = delete;

	virtual void Parse(std::vector<Data>& result, int depth) override;

private:
	Node<Data>* m_pNode;
	int m_Repetitions{ 0 };
};

template<typename Data>
RepetitionNode<Data>::RepetitionNode(Node<Data>* node, int repetitions)
	: m_pNode{ node }
	, m_Repetitions{ repetitions }
{}

template<typename Data>
void RepetitionNode<Data>::Parse(std::vector<Data>& result, int depth) {

	if (depth > MAXDEPTH) {
		return;
	}


	for (int i{ 0 }; i < m_Repetitions; ++i) {

		m_pNode->Parse(result, ++depth);
		--depth;
	}

}