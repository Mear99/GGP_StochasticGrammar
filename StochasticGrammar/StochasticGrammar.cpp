// StochasticGrammar.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Robbe Mahieu

#include <iostream>
#include "Nodes.h"
#include "Grammar.h"

int main()
{
    std::cout << "-- Stochastic grammar demo --\n";

    LeafNode* node = new LeafNode("test");
    Grammar* grammar = new Grammar(node);

    std::cout << grammar->GenerateSequence() << "\n";

    delete node;
    delete grammar;
}
