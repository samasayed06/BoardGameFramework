#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <string>

using namespace std;

class SUS_Board : public Board<char> {
public:
    SUS_Board();

    // core overrides required by the framework
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // helper: return score for given player (by symbol)
    int get_score(Player<char>* player) const;

private:
    int score_S;
    int score_U;

    int count_SUS_at(int x, int y) const;
    bool in_bounds(int x, int y) const;
};

class SUS_UI : public UI<char> {
public:
    SUS_UI() {}
    // create_player is used by the base UI::setup_players()
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    // get_move is used by GameManager to ask for a player's move
    Move<char>* get_move(Player<char>* player) override;
};

#endif // SUS_CLASSES_H
