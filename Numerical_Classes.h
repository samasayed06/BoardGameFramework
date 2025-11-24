#ifndef NUMERICAL_CLASSES_H
#define NUMERICAL_CLASSES_H

#include "BoardGame_Classes.h"
#include <set>
#include <cstdlib>
#include <ctime>
using namespace std;

class Numerical_Board : public Board<int> {
private:
    set<int> usedNumbers;      // Track used numbers
    int blank_symbol = 0;

public:
    Numerical_Board();

    // Getter لإرجاع الأرقام المستخدمة
    const set<int>& getUsedNumbers() const { return usedNumbers; }

    bool update_board(Move<int>* move);
    bool is_win(Player<int>* player);
    bool is_lose(Player<int>* player);
    bool is_draw(Player<int>* player);
    bool game_is_over(Player<int>* player) { return false; }

private:
    bool check_player_line(int playerID);
};


class Numerical_UI : public UI<int> {
public:
    Numerical_UI();

    Player<int>* create_player(string& name, int symbol, PlayerType type);

    Player<int>** setup_players();

    Move<int>* get_move(Player<int>* player);

private:
    int get_random_valid_number(int playerID, const set<int>& used);
};

#endif
