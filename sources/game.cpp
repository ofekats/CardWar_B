#include "game.hpp"
#include <iostream>
#include <iomanip> //for printing like a table
#include <stdio.h>
#include <stdexcept> //for exceptions
#include <unistd.h> //for sleep

using namespace std;
using namespace ariel;

#define NUMBER_OF_CARD 52

//func can throw an exception
Game::Game(Player& p1_ref, Player& p2_ref): //constructor
p1_(p1_ref), p2_(p2_ref)
{
    if(p1_ref.getIsPlaying() == false && p2_ref.getIsPlaying() == false)
    {
        this->gameID = Game::generate_id();
        this->num_of_drow_ =0;
        this->num_of_rounds_=0;
        this->filename = "gameLogs_"+gameID+".txt";
        remove(filename.c_str());
        this->gameLogsFile.open(filename, ios::app); // create file if it doesn't exist, otherwise append to end of file
        if (!this->gameLogsFile.is_open()) 
        {
            throw runtime_error("Failed to open gameLogs file"); // throw an exception
        }
        //change players to playing a game
        this->p1_.setIsPlaying(true);
        this->p2_.setIsPlaying(true);
        //create pack of cards
        Card deck_of_card[NUMBER_OF_CARD]; //array of 52 cards
        Card::cardsDeck_52(deck_of_card, NUMBER_OF_CARD);
        Card::deck_shuffle(deck_of_card);
        //dealing the cards
        for(int i =0; i< NUMBER_OF_CARD; i+=2)
        {
            p1_.push_stack(deck_of_card[i]);
            p2_.push_stack(deck_of_card[i+1]);
        }
    }
    else
    {
        throw invalid_argument("The players are already in a different game"); // throw an exception
    }
}

string Game::generate_id()
{
    string id = "";
    srand(time(NULL));
    for(int i=0; i<5; i++)
    {
        int randomNumber = rand() % 123;
        if(64 < randomNumber && randomNumber < 91 || 96 < randomNumber && randomNumber < 123 )
        {
            char c = randomNumber;
            id+=c;
        }
        else
        {
            id += to_string(randomNumber % 10);
        }
    }
    sleep(1);
    return id;
}

//func can throw an exception
void Game::playTurn()
{
    if(&p1_ != &p2_) //if it is the same player throw exception
    {
        if(p1_.stacksize() > 0 && p2_.stacksize() > 0) //if there are more card we can play
        {
            int nCard_this_round = 0; //cards in the game this round
            bool half = false; //if each player need to get half the cards
            this->num_of_rounds_++; 
            Card c1 = p1_.pop_stack();
            Card c2 = p2_.pop_stack();
            nCard_this_round += 2;
            //gameLogs 
            string p1_card = c1.getCnum_string();
            string p2_card = c2.getCnum_string();
            gameLogsFile << p1_.getName() << " played " << p1_card << " of " << c1.getCshape() << " ";
            gameLogsFile << p2_.getName() << " played " << p2_card << " of " << c2.getCshape() << ". ";
            //drow case:
            while(c1.getCnum_int() == c2.getCnum_int())
            {
                this->num_of_drow_++;
                //gameLogs 
                gameLogsFile << "Drow. ";
                if(p1_.stacksize() == 0 && p2_.stacksize() == 0)
                {
                    half=true; //cards are gone in drow -> each player need to get half the cards
                    //gameLogs 
                    gameLogsFile << "cards are gone- no winner to this round." << endl;
                    break;
                }
                //opposite cards
                p1_.pop_stack();
                p2_.pop_stack();
                nCard_this_round += 2;
                if(p1_.stacksize() == 0 && p2_.stacksize() == 0)
                {
                    half=true; //cards are gone in drow -> each player need to get half the cards
                    //gameLogs 
                    gameLogsFile << "cards are gone- no winner to this round." << endl;
                    break;
                }
                //play again after drow
                c1 = p1_.pop_stack();
                c2 = p2_.pop_stack();
                nCard_this_round += 2;
                //gameLogs
                string p1_card = c1.getCnum_string();
                string p2_card = c2.getCnum_string();
                gameLogsFile << p1_.getName() << " played " << p1_card << " of " << c1.getCshape() << " ";
                gameLogsFile << p2_.getName() << " played " << p2_card << " of " << c2.getCshape() << ". ";
            }
            if(half) //cards are gone in drow -> each player need to get half the cards - end of game
            {
                p1_.addTocardesTaken(nCard_this_round/2);
                p2_.addTocardesTaken(nCard_this_round/2);
                endOfGame(); //finish the game 
            }
            else //not end of game
            {
                int p1_card = c1.getCnum_int();
                int p2_card = c2.getCnum_int();
                //Ace
                if(p1_card == 1)
                {
                    if(p2_card != 2)
                    {
                        p1_card =14;
                    }
                }
                if(p2_card == 1)
                {
                    if(p1_card != 2)
                    {
                        p2_card =14;
                    }
                }

                if(p1_card > p2_card) //player1 won this round
                {
                    p1_.addTocardesTaken(nCard_this_round);
                    p1_.addToRoundWon(1);
                    //gameLogs 
                    gameLogsFile << p1_.getName() << " wins." << endl;
                }
                else //player2 won this round
                {
                    p2_.addTocardesTaken(nCard_this_round);
                    p2_.addToRoundWon(1);
                    //gameLogs 
                    gameLogsFile << p2_.getName() << " wins." << endl;
                }
                if(p1_.stacksize() == 0 && p2_.stacksize() == 0) //cards are gone after this round
                {
                    endOfGame(); //finish the game 
                }
            }
        }
        else
        {
            endOfGame();
            throw invalid_argument("No cards remain to the players"); // throw an exception
        }
    }
    else
    {
        endOfGame();
        throw invalid_argument("One player can't play alone"); // throw an exception
    }
}

//func can throw an exception
void Game::printLastTurn() // print the last turn gameLogs
{
    ifstream file(this->filename);
    if (!file.is_open()) 
    {
        endOfGame();
        throw runtime_error("Failed to open gameLogs file"); // throw an exception
    }
    string lastLine, line;
    while (getline(file, line)) {
        lastLine = line;
    }
    cout << lastLine << endl;
    file.close();
}

void Game::playAll() //playes the game untill the end
{
    while(p1_.stacksize() > 0 && p2_.stacksize() > 0)
    {
        playTurn();
    }
    endOfGame(); //finish the game
}

void Game::printWiner() // prints the name of the winning player
{
    if(p1_.cardesTaken() > p2_.cardesTaken())
    {
        cout << p1_.getName() << " is the winner!" << endl;
    }
    else if (p1_.cardesTaken() < p2_.cardesTaken())
    {
        cout << p2_.getName() << " is the winner!" << endl;
    }
    else
    {
        cout << "tie! no winner" << endl; 
    }
}

//func can throw an exception
void Game::printLog() // prints all the turns played one line per turn
{
    //open the file in read mood
    ifstream file(this->filename);
    if (!file.is_open()) 
    {
        endOfGame();
        throw runtime_error("Failed to open gameLogs file"); // throw an exception
    }
    //print each line 
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void Game::printStats() // for each player prints basic gameLogsistics: win rate, cards won, <other gameLogs you want to print>. Also print the draw rate and amount of draws that happand. (draw within a draw counts as 2 draws. )
{
    int len = 15;
    if(p1_.getName().length() > len)
    {
        len = p1_.getName().length();
    }
    if(p2_.getName().length() > len)
    {
        len = p2_.getName().length();
    }
    cout << left << setw(len) << "Name" << "|"
              << left << setw(len) << "win rate" << "|"
              << left << setw(len) << "cards won" << "\n"
              << setw(len) << p1_.getName() << "|"
              << setw(len) << to_string(static_cast<double>(p1_.getRoundWon())/static_cast<double>(num_of_rounds_)*100)+"%" << "|"
              << setw(len) << p1_.cardesTaken() << "\n"
              << setw(len) << p2_.getName() << "|"
              << setw(len) << to_string(static_cast<double>(p2_.getRoundWon())/static_cast<double>(num_of_rounds_)*100)+"%" << "|"
              << setw(len) << p2_.cardesTaken() << "\n"
              << "\n"
              << left << setw(len) << "amount of round" << "|"
              << setw(len) << "draw rate" << "|"
              << setw(len) << "amount of draws" << "\n"
              << setw(len) << num_of_rounds_ << "|"
              << setw(len) << to_string(static_cast<double>(num_of_drow_)/static_cast<double>(num_of_rounds_)*100)+"%" << "|"
              << setw(len) << num_of_drow_ << "\n";
}

void Game::endOfGame() //remove the players from the game
{
    p1_.setIsPlaying(false);
    p2_.setIsPlaying(false);
    this->gameLogsFile.close();
}