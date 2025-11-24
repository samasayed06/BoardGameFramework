#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
using namespace std;

class SUS_Board : public Board<char> {
private:
    int scoreP1;   // Score for player 1
    int scoreP2;   // Score for player 2

public:
    SUS_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>*) override;
    bool is_lose(Player<char>*) override;
    bool is_draw(Player<char>*) override;
    bool game_is_over(Player<char>*) override;

    int count_SUS_sequences();
    void print_scores();

    int getScore(char symbol);

};

#endif
