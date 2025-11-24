#ifndef TTT5_CLASSES_H
#define TTT5_CLASSES_H

#include "BoardGame_Classes.h"

class TTT5_Board : public Board<char> {
public:
    TTT5_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

private:
    int count_sequences(char sym) const;
};

class TTT5_UI : public UI<char> {
public:
    TTT5_UI();

    Move<char>* get_move(Player<char>* player) override;
};

#endif

