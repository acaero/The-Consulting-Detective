# The-Private-Investigator

#### Description:
This programm is designed to help you win at the boardgame Cluedo.

The logic of the programm is based around three tables which contain information about which player owns which cards.
As soon as you can make the logical conclusion that a person definitely has or does not have a certain card, the programm will find out and write that down.

There are also cases where you can deduce that a person definitely haseither one of three cards.
In that case the programm creates a node for those three cards and checks back every round in order to see if there is new information available that allows you to conclude which of the three cards it was.

Everything the user types in is checked for spelling mistakes which makes it impossible to accidentaly type in invalid information.

The whole programm is stored in one single file but split into several functions.
I didnt want to use CS50s librarys so I first created two functions that take user input (strings and Integers).
Then there are two functions that checks wether or not the user Input is a valid card or player name.


The rest of the functions take care of the logical conclusions needed to solve the murder mistery.
There are several conclusions you can make (for those to make sense you'll need to know the rules of the game):
- If somebody shows you a card, you know that that person has that card.
- If somebody is not able to show a card, you know that that person has none of the card included in the "suggestion".
- If somebody shows somebody else a card, you know that the person that showed a card owns at least one of the cards mentioned in the suggestion. You can use this information later when some of those cards are eliminated.
- If you know all of the cards that somebody has, you can conclude that can't have any of the other cards in the game.
- If you know the owner of all but one card in a given category, it is safe to say that that card was used in the murder.
- If you know that somebody has a certain card, you can conclude that the other players do not have this card.

Most of these are obvious but as a human it is hard to keep track of all of them at the same thime.
Especialy the third point makes the program very effective at winning the game.


I worked a lot with structures that make up nodes which are linked together.
For example here is what the TBC structure does:
//It is a node-structure where each node holds a player and a maximum of 3 cards (one of each category), from which that player definitely has at least one.
//The player and cards are represented as integers, which correspond to the place of the string in the corresponding array.
//When the number 99 is assigned to one of these ints, it means that the person in question does not have this card.
//Once two cards are assigned the value 99 we can therefore deduce that the person must own the third card
You can find detailed explanations like this one in the code of the program, since i tried to be somewhat dilligent when it comes to commenting.

In practice this programm will be used in the following way:
The next time you play the board game Cluedo with your mates you take out your laptop, run the program and smirk confidently.
In the beginning you will have to enter the name of the cards which are used in your version.
Then each round you enter the name of the player whos turn it is, the cards which are included in his suggestion and the player who showed him a card.
If it is your turn you will also be asked to type in the card that you were shown.
Apart from typing that information in you can lay back, put up your feet and let the computer do the rest.
It should then only take a few round until you can declare victory.
