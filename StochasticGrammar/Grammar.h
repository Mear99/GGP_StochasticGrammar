#pragma once
#include <string>
#include "Nodes.h"
#include <memory>
#include <unordered_map>

using weightedRule = std::pair<std::string, float>;
using repeatedRule = std::pair<std::string, int>;

class Grammar
{
	public:
		Grammar();
		virtual ~Grammar() = default;

		Grammar(const Grammar&) = delete;
		Grammar(Grammar&&) = delete;
		Grammar& operator=(const Grammar&) = delete;
		Grammar& operator=(Grammar&&) = delete;

		std::string GenerateSequence(std::string rule);

		void AddLeaf(const std::string& name, const std::string& leaf);
		void ParseRule(const std::string& name, const std::string& rule);
		void AddSelectorRule(const std::string& name, const std::vector<weightedRule> rules);
		void AddSequenceRule(const std::string& name, const std::vector<std::string> rules);
		void AddRepetitionRule(const std::string& name, const repeatedRule rule);

	private:
		std::unordered_map<std::string, std::unique_ptr<Node>> m_pRules;

		void ParseSelectorRule(const std::string& name, std::string& rule);
		void ParseSequenceRule(const std::string& name, std::string& rule);
		void ParseRepetitionRule(const std::string& name, std::string& rule);
		void ParseLeafRule(const std::string& name, std::string rule);
};

