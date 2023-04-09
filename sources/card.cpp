#include "card.hpp"
#include <cstdlib> //for random
#include <ctime> //for time
#include <iostream>
#include <stdexcept> //for exceptions


using namespace std;
using namespace ariel;

//from enum shape to string type
string Card::shapeToString(Card::Shape shape) {
    switch (shape) {
        case Card::Diamonds:
            return "Diamonds";
        case Card::Clubs:
            return "Clubs";
        case Card::Hearts:
            return "Hearts";
        case Card::Spades:
            return "Spades";
        default:
            return "Invalid Shape";
    }
}

Card::Card() //defult constructor
{
    this->cardNum_ = 1;
    this->cardShape_ = "Diamonds";
}

Card::Card(int num, string shape) //constructor
{
    this->cardNum_ = num;
    this->cardShape_ = shape;
}

int Card::getCnum_int() const{ //return the card number as int
    return this->cardNum_;
}

string Card::getCnum_string() const{ //return the card number as string
    switch (this->cardNum_) {
        case 1:
            return "Ace";
        case 11:
            return "Jack";
        case 12:
            return "Queen";
        case 13:
            return "King";
        default:
            return to_string(this->cardNum_);
    }
}

string Card::getCshape() const{ //return the card shape
    return this->cardShape_;
}

//func can throw an exception
void Card::cardsDeck_52(Card* cardsDeck, int size) //create a deck of 52 card - no joker
{
    if(size != 52)
    {
        throw invalid_argument("deck of card is 52 cards"); // throw an exception
    }
    int card_num = 0;

    for(int i=0; i<size; i+=4)
    {
        card_num ++;
        for(int j=0; j<4; j++)
        {
            //enum of shapes
            Shape s = static_cast<Shape>(j+1);
            Card c(card_num, shapeToString(s));
            cardsDeck[i+j] = c;
        }
    }
}

void Card::deck_shuffle(Card* cardsDeck) //shuffle a deck of cards
{
    // Seed the random number generator with the current time
    srand(time(nullptr));
    
    for(int i=0; i<250; i++)
    {
        // Generate 2 random numbers between 0 and 51
        int index1 = rand() % 52;
        int index2 = rand() % 52;
        deck_swap(cardsDeck, index1, index2);
    }
    
}

void Card::deck_swap(Card* cardsDeck, int index1, int index2) //swap index i and j in cardsdeck
{
    Card temp = cardsDeck[index1];
    cardsDeck[index1] = cardsDeck[index2];
    cardsDeck[index2] = temp;
}

void Card::CardToString()
{
    cout << this->getCnum_string() << " of " << this->getCshape() << endl;
}