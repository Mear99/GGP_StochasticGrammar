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
    SequenceNode* nodeSequence = new SequenceNode();

    nodeSequence->AddElement(node);
    nodeSequence->AddElement(nodeSelect);

    nodeSelect->AddOption(nodeSequence);
    nodeSelect->AddOption(nodeSequence);
    nodeSelect->AddOption(node2);

    Grammar* grammar = new Grammar(nodeSequence);

    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";
    std::cout << grammar->GenerateSequence() << "\n";

    delete node;
    delete node2;
    delete nodeSelect;
    delete grammar;
}
