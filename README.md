# Stochastic Grammar (Gameplay Programming DAE)

![Rivers](https://user-images.githubusercontent.com/48439256/213487819-5ea514a6-3048-49b4-852a-a953967aa1ba.gif)

## Introduction
A grammar is a set of structural constraints for creating phrases. We use it every day in conversations and each language has it's own grammar. This also means we can create our own set of rules and create phrases that adhere to those defined rules. This is exactly what this project is about. The focus here will be on stochastic grammar, or grammar with some 'randomness' or probability in the rules. The goal will be to make a framework for creating stochastic grammars and using that framework to compose grammars showing different applications in game development.

## Creating grammar
Let's start simple: Say we want to generate a set 3 of tasks. We need a rule that decribes all the possible tasks and a rule that picks three of those. let's give those rules a name, 'Task' and 'Select' respectively. These two rules have some kind of relationship, namely 'Select' will use 'Task' three times. Let's represent this grafically: 

<img src="https://user-images.githubusercontent.com/48439256/213495374-c4f1c691-be54-4aee-aa73-b58395c6e397.png" width=50% height=50%>

Say we want to generate two of these lists, we need another rule (let's call it generate). Let's add this to the representation:

<img src="https://user-images.githubusercontent.com/48439256/213496360-2d6c1024-1c2c-45f3-8c10-f683386b7f44.png" width=50% height=50%>

This starts to look a lot like a tree, so that will be how we will structure the grammar! We will have a couple of different types of nodes in our grammar tree that will allow us to define different rules. A phrase will be represented as a vector and when the grammar has to generate a phrase, the grammar tree will be traversed and the nodes will append the correct elements to this phrase vector.

### Leaf node
This node will always be at the end of a branch and contains our payload. In most case, this will be a string but the framework will allow you to add any type as the payload. This allows us to even add a callable object as the payload, which is uses in the Rivers and L-System example. This payload will be added to the phrase vector.

### Sequence node
This node contains a list of other nodes it will execute in order. It will add the result of each of these 'childnodes' to the phrase vector.

### Selector node
This node contains a list of other nodes. It will pick one of these childnodes and add it's result to the phrase vector.

## Allowing for randomness
We now has a framework that supports a basic grammar, but there is almost no probability involved. Only the selector node will pick a random childnode, but right now each childnode has the same chance to be picked. Let's add some weight values to that selector node and introduce some other nodes to allow for more randomness.

### Selector node
Each node will be paired with a weight. The bigger the weight, the more likely it will be picked. Each time a childnode is needed, the weightd of all the childnodes are summed up, and a random number between 0 and this sum is picked. The for each node, it is checked if the weight is bigger than this random number. If this is the case, we've found our childnode! If not, we substract the weight from this random number and keep going.

### Repetition node
This node will repeat another node based on a chance. Every time the childnode is executed, it's result will be added ti the phrase vector.

## Adding recursion
Now all this randomness can lead to phrases that keep generating due to recursion. One node has a chance to call another node, which calls the first node again and so on and so on. To prevent this, I've added a node that supports recursion safely by setting a recursion depth. Once this depth is reached, the node will call a fallback node and that will be added to the phrase vector. For simple grammars, this node isn't really needed, but more complex grammars will have more recursion and will have more chance to reach the end of the stack and cause a stack overflow. This recursion node will also reduce the risk of that happening. 

## Applications
### L-Systems

### River generation

### Shop generation

## Conclusion

## References
This research was based on [this paper](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter36_Stochastic_Grammars_Not_Just_for_Words.pdf) published in Game AI Pro 3.
