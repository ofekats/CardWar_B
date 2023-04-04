#pragma once

#include <string>

using namespace std;

namespace ariel{
    class Card{
        public:
            enum Shape {Diamonds =1, Clubs, Hearts, Spades};
            static string shapeToString(Shape shape);
            Card(); //defult constructor
            Card(int num, string shape); //constructor
            int getCnum_int() const; //return the card number as int
            string getCnum_string() const; //return the card number as string
            string getCshape() const; //return the card shape
            static void cardsDeck_52(Card* cardsDeck, int size); //create a deck of 52 card - no joker
            static void deck_shuffle(Card* cardsDeck); //shuffle a deck of cards
            static void deck_swap(Card* cardsDeck, int index1, int index2); //swap index1 and index2 in cardsDeck
            void CardToString();
        private:
            int cardNum_;
            string cardShape_;
    };
}
