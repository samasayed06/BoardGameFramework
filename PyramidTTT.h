#ifndef _PYRAMID_TTT_CLASSES_H
#define _PYRAMID_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class PyramidTTT_Board : public Board<char> {
public:
    PyramidTTT_Board();
    virtual ~PyramidTTT_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;

    bool has_three_in_a_row(char mark);

    // static helper لدعم Smart AI
    static bool has_three_in_a_row_on_matrix(const vector<vector<char>>& mat, char mark);

private:
    bool is_valid_position(int r, int c);
};

class PyramidTTT_Human : public Player<char> {
public:
    PyramidTTT_Human(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class PyramidTTT_Computer : public Player<char> {
public:
    PyramidTTT_Computer(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class PyramidTTT_UI : public UI<char> {
public:
    PyramidTTT_UI();

    Move<char>* get_move(Player<char>* p) override;

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};


#endif