// StochasticGrammar.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Robbe Mahieu

#if defined(_DEBUG)
#include "vld.h"
#endif

#include <iostream>
#include "Nodes.h"
#include "Grammar.h"

int main()
{
    std::cout << "-- Stochastic grammar demo --\n";

    Grammar<std::string>* grammar = new Grammar<std::string>();

    grammar->ParseRule("a", "a & b");
    grammar->ParseRule("b", "a");

    auto result{ grammar->GenerateSequence("a") };

    std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << "\n";

    delete grammar;

}
