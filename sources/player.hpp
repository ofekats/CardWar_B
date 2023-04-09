#pragma once

#include <string>
#include <stack>
#include "card.hpp"

using namespace std;

namespace ariel{
    class Player{
    public:
        Player(string name); //constructor

        int stacksize() const; //return the amount of cards left (size_of_stack_)
        void setstacksize(int size); //change the size_of_stack_
        void subStacksize(int tosub); //decrease by tosub the size_of_stack_

        int cardesTaken() const; //return the amount of cards this player has won (num_of_win_cards_)
        void setcardesTaken(int num); //change the num_of_win_cards_
        void addTocardesTaken(int toadd); //increase by toadd the num_of_win_cards_

        string getName () const; //return the player name

        Card pop_stack();
        void push_stack(Card c_to_push);

        int getRoundWon() const; //return the amount of rounds this player has won (num_of_win_round_)
        void setRoundWon(int num); //change the num_of_win_round_
        void addToRoundWon(int toadd); //increase by toadd the num_of_win_round_
        
        bool getIsPlaying() const; //is this player in a game or not
        void setIsPlaying(bool state); //change if this player in a game or done

    private:
        string name_;
        int num_of_win_cards_;
        int size_of_stack_;
        stack<Card> player_stack_;
        int num_of_win_round_;
        bool isPlaying;
    };
}