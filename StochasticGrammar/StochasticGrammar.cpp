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

    Grammar<std::string>* fern = new Grammar<std::string>();

    fern->ParseRule("X", " X -> F & + & [ & [ & X & ] & - & X & ] & - & F & [ & - & F & X & ] & + & X");
    fern->ParseRule("F", "F -> F & F");

    auto result{ fern->GenerateSequence("X") };

    std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << "\n";

    delete fern;

}
