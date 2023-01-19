# Stochastic Grammar (Gameplay Programming DAE)

![Rivers](https://user-images.githubusercontent.com/48439256/213487819-5ea514a6-3048-49b4-852a-a953967aa1ba.gif)

## Introduction
A grammar is a set of structural constraints for creating phrases. We use it every day in conversations and each language has it's own grammar. This also means we can create our own set of rules and create phrases that adhere to those defined rules. This is exactly what this project is about. The focus here will be on stochastic grammar, or grammar with some 'randomness' or probability in the rules. The goal will be to make a framework for creating stochastic grammars and using that framework to compose grammars showing different applications in game development.

## Creating grammar
Let's start simple: Say we want to generate a set 3 of tasks. We need a rule that decribes all the possible tasks and a rule that picks three of those. let's give those rules a name, 'Task' and 'Select' respectively. These two rules have some kind of relationship, namely 'Select' will use 'Task' three times. Let's represent this grafically: 
![basicGrammarTree](https://user-images.githubusercontent.com/48439256/213495374-c4f1c691-be54-4aee-aa73-b58395c6e397.png | width=100)

Say we want to generate two of these lists, we need another rule (let's call it generate). Let's add this to the representation:
![extGrammarTree](https://user-images.githubusercontent.com/48439256/213496360-2d6c1024-1c2c-45f3-8c10-f683386b7f44.png | width=100)

This starts to look a lot like a tree, so that will be how we will structure the grammar! We will have a couple of different types of nodes in our grammar tree that will allow us to define different rules.

### Leaf node
### Sequence node
### Selector node

## Allowing for randomness
### Selector node
### Repetition node

## Adding recursion

## Applications
### L-Systems

### River generation

### Shop generation

## Conclusion

## References
This research was based on [this paper](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter36_Stochastic_Grammars_Not_Just_for_Words.pdf) published in Game AI Pro 3.
