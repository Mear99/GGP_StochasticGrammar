// StochasticGrammar.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Robbe Mahieu

#include <iostream>
#include "Nodes.h"
#include "Grammar.h"

int main()
{
    std::cout << "-- Stochastic grammar demo --\n";

    LeafNode* node = new LeafNode("test");
    LeafNode* node2 = new LeafNode("alt");
    SelectNode* nodeSelect = new SelectNode();
    nodeSelect->AddOption(node);
    nodeSelect->AddOption(node2);
    Grammar* grammar = new Grammar(nodeSelect);

    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";

    delete node;
    delete node2;
    delete nodeSelect;
    delete grammar;
}
