#pragma once
#include "player.hpp"
#include "card.hpp"
#include <fstream> //for file


namespace ariel{
    class Game{
    public:
        // Game(); //defult constructor
        Game(Player& p1_ref, Player& p2_ref); //constructor
        ~Game();
        void playTurn(); 
        void printLastTurn(); // print the last turn gameLogs
        void playAll(); //playes the game untill the end
        void printWiner(); // prints the name of the winning player
        void printLog(); // prints all the turns played one line per turn
        void printStats(); // for each player prints basic statistics: win rate, cards won, <other gameLogs you want to print>. Also print the draw rate and amount of draws that happand. (draw within a draw counts as 2 draws. )
        void endOfGame(); //remove tje players from the game
    private:
        Player& p1_;
        Player& p2_;
        int num_of_drow_;
        int num_of_rounds_;
        ofstream gameLogsFile;
        string filename;
        static int numGamesPlayed;
    };
}


