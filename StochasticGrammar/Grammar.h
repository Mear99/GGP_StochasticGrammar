#pragma once
#include <string>
#include "Nodes.h"
#include <memory>
#include <unordered_map>

#define SEQ_DEL " & "
#define SEL_DEL " | "
#define REP_DEL " # "

using weightedRule = std::pair<std::string, float>;
using repeatedRule = std::pair<std::string, int>;

class Rule404Exception {};

template<typename Data>
class Grammar
{
	public:
		Grammar();
		virtual ~Grammar() = default;

		Grammar(const Grammar&) = delete;
		Grammar(Grammar&&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		Grammar& operator=(Grammar&&) = delete;

		std::vector<Data> GenerateSequence(std::string rule);

		void AddLeaf(const std::string& name, const Data& data);
		void ParseRule(const std::string& name, const std::string& rule);
		void AddSelectorRule(const std::string& name, const std::vector<weightedRule> rules);
		void AddSequenceRule(const std::string& name, const std::vector<std::string> rules);
		void AddRepetitionRule(const std::string& name, const repeatedRule rule);

	private:
		std::unordered_map<std::string, std::unique_ptr<Node<Data>>> m_pRules;

		void ParseSelectorRule(const std::string& name, std::string& rule);
		void ParseSequenceRule(const std::string& name, std::string& rule);
		void ParseRepetitionRule(const std::string& name, std::string& rule);
		void ParseLeafRule(const std::string& name, std::string rule);
};

template<typename Data>
Grammar<Data>::Grammar()
{}

template<typename Data>
std::vector<Data> Grammar<Data>::GenerateSequence(std::string ruleName) {

	if (m_pRules.find(ruleName) == m_pRules.end()) {
		throw Rule404Exception{};
	}

	std::vector<Data> result{};
	m_pRules[ruleName]->Parse(result, 0);
	return result;
}

// Needs some kind of conversion from string to data...
// Mostly add leaf nodes manually
void Grammar<std::string>::AddLeaf(const std::string& name, const std::string& leaf) {
	m_pRules[name] = std::make_unique<LeafNode<std::string>>(leaf);
}

template<typename Data>
void Grammar<Data>::AddLeaf(const std::string& name, const Data& data) {

	// Non-existing rule
	m_pRules[name] = std::make_unique<LeafNode<Data>>(data);
}


template<typename Data>
void Grammar<Data>::AddSelectorRule(const std::string& name, const std::vector<weightedRule> rules) {
	std::unique_ptr<SelectNode<Data>> selectNode = std::make_unique<SelectNode<Data>>();
	for (auto& rule : rules) {

		// Non-existing subrule
		if (m_pRules.find(rule.first) == m_pRules.end()) {
			ParseRule(rule.first, rule.first);
		}

		// Add to selectNode
		selectNode->AddOption(m_pRules[rule.first].get(), rule.second);
	}

	// Non-existing rule
	m_pRules[name] = std::move(selectNode);
}

template<typename Data>
void Grammar<Data>::AddSequenceRule(const std::string& name, const std::vector<std::string> rules) {
	std::unique_ptr<SequenceNode<Data>> sequenceNode = std::make_unique<SequenceNode<Data>>();
	for (auto& rule : rules) {

		// Non-existing subule
		if (m_pRules.find(rule) == m_pRules.end()) {
			ParseRule(rule, rule);
		}

		// Add to selectNode
		sequenceNode->AddElement(m_pRules[rule].get());
	}

	// Non-existing rule
	m_pRules[name] = std::move(sequenceNode);
}

template<typename Data>
void Grammar<Data>::AddRepetitionRule(const std::string& name, const repeatedRule rule) {

	// Non-existing subrule
	if (m_pRules.find(rule.first) == m_pRules.end()) {
		ParseRule(rule.first, rule.first);
	}

	// Non-existing rule
	m_pRules[name] = std::make_unique<RepetitionNode<Data>>(m_pRules[rule.first].get(), rule.second);
}


// Rule syntax:
// Leafnodes are also considered rules
// Each string refers to another rule
// Rules need to be seperated by an operator:
//		sequence: [rule] & [rule]
//		selector: [weight] [rule] | [weight] [rule]
//		repetiton: [rule] # [times]

template<typename Data>
void Grammar<Data>::ParseRule(const std::string& name, const std::string& rule) {

	std::string parsedRule{ rule };

	if (parsedRule.find(SEQ_DEL) != std::string::npos) {
		ParseSequenceRule(name, parsedRule);
		return;
	}
	if (parsedRule.find(SEL_DEL) != std::string::npos) {
		ParseSelectorRule(name, parsedRule);
		return;
	}
	if (parsedRule.find(REP_DEL) != std::string::npos) {
		ParseRepetitionRule(name, parsedRule);
		return;
	}
	ParseLeafRule(name, rule);
}

template<typename Data>
void Grammar<Data>::ParseSelectorRule(const std::string& name, std::string& rule) {

	//split into subrules
	std::vector<std::string> rules;
	size_t first;
	size_t last = 0;

	while ((first = rule.find_first_not_of(SEL_DEL, last)) != std::string::npos) {
		last = rule.find(SEL_DEL, first);
		rules.push_back(rule.substr(first, last - first));
	}

	// turn to weighted rule
	std::vector<weightedRule> weightedRules;
	for (const std::string& rule : rules) {

		size_t spaceIndex = rule.find(" ");
		std::string weightString{ rule.substr(0, spaceIndex) };
		std::string ruleName{ rule.substr(spaceIndex + 1, rule.length() - weightString.length() - 1) };

		float weight{ std::stof(weightString) };
		weightedRules.push_back(std::make_pair(ruleName, weight));
	}

	// Add to grammar
	AddSelectorRule(name, weightedRules);
}

template<typename Data>
void Grammar<Data>::ParseSequenceRule(const std::string& name, std::string& rule) {

	//split into subrules
	std::vector<std::string> rules;
	size_t first;
	size_t last = 0;

	while ((first = rule.find_first_not_of(SEQ_DEL, last)) != std::string::npos) {
		last = rule.find(SEQ_DEL, first);
		rules.push_back(rule.substr(first, last - first));
	}

	AddSequenceRule(name, rules);
}

template<typename Data>
void Grammar<Data>::ParseLeafRule(const std::string& name, std::string rule) {
	AddLeaf(name, rule);
}

template<typename Data>
void Grammar<Data>::ParseRepetitionRule(const std::string& name, std::string& rule) {

	std::string del{ REP_DEL };
	size_t splitIndex = rule.find(del);
	std::string ruleName{ rule.substr(0, splitIndex) };
	std::string repetitionString{ rule.substr(splitIndex + del.length(), rule.length() - ruleName.length() - del.length())};

	int repetitions{ std::stoi(repetitionString) };
	AddRepetitionRule(name, std::make_pair(ruleName, repetitions));
}