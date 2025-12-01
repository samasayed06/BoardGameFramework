#ifndef FOURINAROW_CLASSES_H
#define FOURINAROW_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>
#include <string>

class FourInARow_Board : public Board<char> {
public:
    FourInARow_Board(); // 6 rows x 7 columns

    // Board interface
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // AI helpers
    std::pair<int, int> random_move(char symbol) const; // returns {row, col}
    std::pair<int, int> smart_move(char symbol) const;  // minimax/backtracking

    // helpers used by UI
    int lowest_row_in_column(int col) const;
    std::vector<int> legal_columns() const;

private:
    // helpers
    bool in_bounds(int r, int c) const { return r >= 0 && r < rows && c >= 0 && c < columns; }
    bool check_direction(int r, int c, int dr, int dc, char sym) const; // check 4-in-line containing (r,c) along direction
    bool has_four_for(char sym) const;

    // minimax helpers
    int minimax(int depth, bool maximizingPlayer, char playerSym, char oppSym, int alpha, int beta);
    int evaluate_board(char playerSym, char oppSym) const;
};

class FourInARow_UI : public UI<char> {
public:
    FourInARow_UI();
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif // FOURINAROW_CLASSES_H
