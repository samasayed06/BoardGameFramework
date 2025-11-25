#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
using namespace std;

class SUS_Board : public Board<char> {
private:
    int score_S;
    int score_U;

    bool in_bounds(int x, int y) const;
    int count_SUS_at(int x, int y) const;

public:
    SUS_Board();

    bool update_board(Move<char>* move) override;

    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    int get_score_for_symbol(char s) const;

    pair<int, int> random_move() const;
    pair<int, int> smart_move(char symbol) const;
};

class SUS_UI : public UI<char> {
private:
    bool firstPlayer = true;

public:
    SUS_UI() {}
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif
