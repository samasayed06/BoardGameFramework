/*#ifndef NUMERICAL_CLASSES_H
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

#endif*/






#ifndef NUMERICAL_CLASSES_H
#define NUMERICAL_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <utility>

class Numerical_Board : public Board<int> {
public:
    Numerical_Board();

    // framework overrides
    bool update_board(Move<int>* move) override; // move contains x,y and value
    bool is_win(Player<int>* player) override;
    bool is_lose(Player<int>* player) override;
    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;

    // helper accessors
    std::vector<int> available_numbers_for_player(char player_symbol) const;
    std::pair<int,int> random_move_for_player(char player_symbol) const;
    std::pair<int,int> smart_move_for_player(char player_symbol) const;

    // board representation accessible for AI
private:
    int grid[3][3]; // -1 empty, else value placed
    bool used[10];  // numbers 0..9 used or not
    bool in_bounds(int x,int y) const;

    // evaluation helpers
    bool line_is_sum15_with(int x, int y, int val) const;
};

class Numerical_UI : public UI<int> {
public:
    Numerical_UI() {}
    Player<int>* create_player(std::string& name, int symbol, PlayerType type) override;
    Move<int>* get_move(Player<int>* player) override;
};

#endif

