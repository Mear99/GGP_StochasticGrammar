// StochasticGrammar.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Robbe Mahieu

#include <iostream>
#include "Nodes.h"
#include "Grammar.h"

int main()
{
    std::cout << "-- Stochastic grammar demo --\n";

    Grammar* grammar = new Grammar();

    grammar->ParseRule("options", "0.5 test | 0.5 alt");
    grammar->ParseRule("decide", "options # 5");

    for (int i{ 0 }; i < 10; ++i) {
        std::cout << grammar->GenerateSequence("decide") << "\n";
    }

    delete grammar;
}
