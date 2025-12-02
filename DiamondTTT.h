#ifndef _DIAMOND_TTT_CLASSES_H
#define _DIAMOND_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

class DiamondTTT_Board : public Board<char> {
public:
    DiamondTTT_Board();
    virtual ~DiamondTTT_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;

    bool lines_in_different_directions(char mark);
    bool is_valid_cell(int r, int c) const;  // للتحقق من الخلايا الصالحة في الماسة

    // static helper لدعم Smart AI
    static bool lines_in_different_directions_on_matrix(const vector<vector<char>>& mat, char mark);

private:
    // يمكن إضافة أي private helpers إذا احتجت
};

class DiamondTTT_Human : public Player<char> {
public:
    DiamondTTT_Human(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class DiamondTTT_Computer : public Player<char> {
public:
    DiamondTTT_Computer(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class DiamondTTT_UI : public UI<char> {
public:
    DiamondTTT_UI();
    Move<char>* get_move(Player<char>* p) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif
