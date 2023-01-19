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

    Grammar<std::string>* shop = new Grammar<std::string>();

    shop->ParseRule("Weapon", " 0.1 Sword | 0.1 Daggar | 0.1 Bow | 0.1 Shield");
    shop->ParseRule("Food", " 0.1 Steak | 0.1 Chicken | 0.1 Icecream | 0.1 Juice");
    shop->ParseRule("Armor", "ArmorPiece & of & Magic");
    shop->ParseRule("ArmorPiece", "0.1 Helmet | 0.1 Curass | 0.1 Gloves | 0.1 Sneakers");
    shop->ParseRule("Magic", " 0.1 Healing | 0.1 Stealth | 0.1 Damage | 0.1 Confidence");
    shop->ParseRule("Potion", "potion & of & Magic");
    shop->ParseRule("Type", " 0.1 Weapon | 0.2 Food |  0.7 Armor |  0.1 Potion");
    shop->ParseRule("Cost", "10 10 | 10 100 | 10 200 | 10 500 | 10 250 | 10 500 | 10 1000");
    shop->ParseRule("Quality", "0.1 Perfect | 0.3 Good | 0.5 Decent | 0.3 Bad");
    shop->ParseRule("Rarity", "0.1 Legendary | 0.3 Rare | 0.5 Uncommon | 1.0 Common");
    shop->ParseRule("Item", "Rarity & Quality & Type & Price: & Cost & Gold & coins \n");
    shop->ParseRule("Shop", "Item # 0.9");


    auto result{ shop->GenerateSequence("Shop") };
    
    std::cout << "\n-- Shop generated with Grammar --\n";

    std::cout << " ";
    std::copy(result.begin(), result.end(), std::ostream_iterator<std::string>(std::cout, " "));

    std::cout << "---------------------------------\n\n";
    delete shop;

}
