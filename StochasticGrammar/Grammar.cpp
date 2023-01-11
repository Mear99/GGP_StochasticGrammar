#include "Grammar.h"
#include <cctype>

#define SEQ_DEL '&'
#define SEL_DEL '|'
#define REP_DEL '#'

class Rule404Exception{};

Grammar::Grammar()
{}

std::string Grammar::GenerateSequence(std::string ruleName) {

	if (m_pRules.find(ruleName) == m_pRules.end()) {
		throw Rule404Exception{};
	}

	return m_pRules[ruleName]->Parse();
}

void Grammar::AddLeaf(const std::string& name, const std::string& leaf) {
	m_pRules[name] = std::make_unique<LeafNode>(leaf);
}


void Grammar::AddSelectorRule(const std::string& name, const std::vector<weightedRule> rules) {
	std::unique_ptr<SelectNode> selectNode = std::make_unique<SelectNode>();
	for (auto& rule : rules) {

		// Non-existing Rule?
		if (m_pRules.find(rule.first) == m_pRules.end()) {
			ParseRule(rule.first, rule.first);
		}

		// Add to selectNode
		selectNode->AddOption(m_pRules[rule.first].get(), rule.second);
	}

	m_pRules[name] = std::move(selectNode);
}

void Grammar::AddSequenceRule(const std::string& name, const std::vector<std::string> rules) {
	std::unique_ptr<SequenceNode> sequenceNode = std::make_unique<SequenceNode>();
	for (auto& rule : rules) {

		// Non-existing Rule?
		if (m_pRules.find(rule) == m_pRules.end()) {
			ParseRule(rule, rule);
		}

		// Add to selectNode
		sequenceNode->AddElement(m_pRules[rule].get());
	}

	m_pRules[name] = std::move(sequenceNode);
}

void Grammar::AddRepetitionRule(const std::string& name, const repeatedRule rule) {

	// Non-existing Rule?
	if (m_pRules.find(rule.first) == m_pRules.end()) {
		ParseRule(rule.first, rule.first);
	}

	m_pRules[name] = std::make_unique<RepetitionNode>(m_pRules[rule.first].get(), rule.second);
}


// Rule syntax:
// Leafnodes are also considered rules
// Each string refers to another rule
// Rules need to be seperated by an operator:
//		sequence: [rule] & [rule]
//		selector: [weight] [rule] | [weight] [rule]
//		repetiton: [rule] # [times]


void Grammar::ParseRule(const std::string& name, const std::string& rule) {
	
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

void Grammar::ParseSelectorRule(const std::string& name, std::string& rule) {

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

		size_t spaceIndex = rule.find(" ", 1);
		std::string weightString{ rule.substr(0, spaceIndex) };
		std::string ruleName{ rule.substr(spaceIndex + 1, rule.length() - weightString.length() - 1) };

		// remove spaces
		ruleName.erase(std::remove_if(ruleName.begin(), ruleName.end(), std::isspace), ruleName.end());
		weightString.erase(std::remove_if(weightString.begin(), weightString.end(), std::isspace), weightString.end());

		float weight{ std::stof(weightString) };
		weightedRules.push_back(std::make_pair(ruleName, weight));
	}

	// Add to grammar
	AddSelectorRule(name, weightedRules);
}

void Grammar::ParseSequenceRule(const std::string& name, std::string& rule) {

	// remove spaces
	rule.erase(std::remove_if(rule.begin(), rule.end(), std::isspace), rule.end());

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

void Grammar::ParseLeafRule(const std::string& name, std::string rule) {
	AddLeaf(name, rule);
}

void Grammar::ParseRepetitionRule(const std::string& name, std::string& rule) {
	
	size_t splitIndex = rule.find(REP_DEL, 1);
	std::string ruleName{ rule.substr(0, splitIndex) };
	std::string repetitionString{ rule.substr(splitIndex + 1, rule.length() - ruleName.length() - 1) };

	// remove spaces
	ruleName.erase(std::remove_if(ruleName.begin(), ruleName.end(), std::isspace), ruleName.end());
	repetitionString.erase(std::remove_if(repetitionString.begin(), repetitionString.end(), std::isspace), repetitionString.end());

	int repetitions{ std::stoi(repetitionString) };
	AddRepetitionRule(name, std::make_pair(ruleName, repetitions));
}