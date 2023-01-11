// StochasticGrammar.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Robbe Mahieu

#include <iostream>
#include "Nodes.h"
#include "Grammar.h"

int main()
{
    std::cout << "-- Stochastic grammar demo --\n";

    Grammar<std::string>* grammar = new Grammar<std::string>();

    grammar->ParseRule("options", "0.5 test | 0.5 alt");
    grammar->ParseRule("root", "0.2 options # 3 | 0.8 options # 8");

    for (int i{ 0 }; i < 10; ++i) {
        auto result{ grammar->GenerateSequence("root") };

        std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << "\n";
    }

    delete grammar;
}
