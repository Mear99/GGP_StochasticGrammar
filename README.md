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

## Parsing rules as strings
I also added the functionality to the framework to create the rules based on a string instead of adding every node manually. The parser will automatically create the rule from a string. Here's how the parsing works:
* Each string is a single rule
* Leafnodes are also considered rules
  - *If there are not strings, they will need to be added manually!*
* Rules need to be seperated by an operator:
  - sequence: [rule] & [rule]
  -	selector: [weight] [rule] | [weight] [rule]
  -	repetiton: [rule] # [times]
  -	recursion: [fallback rule] -> [normal rule]

## Applications
### L-Systems
I needed a way to confirm if the grammar was working correctly. So I looked for some examples of grammar generated sequences and came across L-Systems. L-Systems are an example of formal grammar and can create self-similar fractals. So I tested out the framwork by creating some grammars that mimic the rules of some L-Systems, in particular the fractal plant. L-Systems use recursion to generate the phrase and the max recursion depth is in this context also called iterations. To visualize the result, each leaf node coincides with a drawing action (a function!), slowly creating the fractal.
Here you can see the influence the recursion depth has on the fractal, which also illustrates why it could also be called iterations.

![LSystemGif](https://user-images.githubusercontent.com/48439256/213525137-a409a690-1b95-47b0-941a-eceb74aab8b8.gif)

### River generation
Now I had this L-System set up, but this does not use a stochastic grammar! So the next step was to add some randomness to it. So I copied over the drawing functions from the L-Systems and wrote my own grammar that uses those functions to generate a random river. I added option for how the rivier should proceed (keep going, turn or split up) and I've given the some weights. Even with such simple grammar, the result is quite good! Here, you can also see the strength of stochastic grammars: Tweaking the weights will give you very different results and you can fine tune them for your application.

![Rivers](https://user-images.githubusercontent.com/48439256/213487819-5ea514a6-3048-49b4-852a-a953967aa1ba.gif)

### Shop generation
Now let's go back to a simple textbased example: let's make a grammar that can generate the contents of a shop. Here you can see the different rule I created for this shop to generate as well as one possible shop create from this grammar:

<img src="https://user-images.githubusercontent.com/48439256/213527104-35272f36-72bd-44ae-ba8b-0c7f12e71891.png" width=50% height=50%>
<img src="https://user-images.githubusercontent.com/48439256/213526915-4a17af65-d2c4-4741-9cf1-7bf9573f83c7.png" width=50% height=50%>

## Conclusion
All in all, grammar can be used for a variety of things, especially in generating things. And stochastic grammar are very powerful here, since it allows for probability to play a role. This results in generating random sequences following structured rules, or, in other word, creating structured randomness! I've dabble with different applications ranging from river generation to creating a shop. This only is a small sample of what is possible: the tree-like structure could allow stochastic grammar to generate behaviour trees, one could generate different styles of enemy behaviour,... .
Still this framework can be expanded:
- The parser could be expanded to allow for nesting rules with parentheses
- The framework could allow grammar to be loaded in from a text file
- New types of nodes could be added, like a node that switches between two rule every time it gets executed
- ...

## References
This research was based on [this paper](http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter36_Stochastic_Grammars_Not_Just_for_Words.pdf) published in Game AI Pro 3.
