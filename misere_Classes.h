#ifndef _MISERE_TTT_CLASSES_H
#define _MISERE_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
using std::string;

class Misere_Board : public Board<char> {
public:
    Misere_Board();
    virtual ~Misere_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;
private:
    bool has_three_in_row(char mark);
};

class Misere_Human : public Player<char> {
public:
    Misere_Human(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class Misere_Computer : public Player<char> {
public:
    Misere_Computer(string name, char symbol);
    Move<char>* get_move(Player<char>*);
};

class Misere_UI : public UI<char> {
public:
    Misere_UI();
    Move<char>* get_move(Player<char>* p) override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
};

#endif

