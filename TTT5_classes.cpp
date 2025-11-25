
#include "TTT5_Classes.h"
#include <iostream>
using namespace std;

// ================== TTT5 Board ======================

TTT5_Board::TTT5_Board() : Board<char>(5, 5) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            board[i][j] = '-';   // empty cell
}

bool TTT5_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= 5 || y < 0 || y >= 5) {
        cout << "Invalid position.\n";
        return false;
    }

    if (board[x][y] != '-') {
        cout << "Cell is not empty!\n";
        return false;
    }

    board[x][y] = sym;
    n_moves++;
    return true;
}

// Count how many 3-in-a-row sequences for symbol
int TTT5_Board::count_sequences(char sym) const {
    int count = 0;

    // rows
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == sym && board[i][j + 1] == sym && board[i][j + 2] == sym)
                count++;

    // columns
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == sym && board[i + 1][j] == sym && board[i + 2][j] == sym)
                count++;

    // diagonal "\"
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == sym && board[i + 1][j + 1] == sym && board[i + 2][j + 2] == sym)
                count++;

    // diagonal "/"
    for (int i = 0; i < 3; i++)
        for (int j = 2; j < 5; j++)
            if (board[i][j] == sym && board[i + 1][j - 1] == sym && board[i + 2][j - 2] == sym)
                count++;

    return count;
}

bool TTT5_Board::is_win(Player<char>* player) {
    if (n_moves < 24)
        return false; // Game not finished yet

    char sym = player->get_symbol();
    char other = (sym == 'X') ? 'O' : 'X';

    int playerSeq = count_sequences(sym);
    int otherSeq = count_sequences(other);

    return playerSeq > otherSeq;
}

bool TTT5_Board::is_lose(Player<char>* player) {
    return false; // no losing condition
}

bool TTT5_Board::is_draw(Player<char>* player) {
    if (n_moves < 24)
        return false;

    char sym = player->get_symbol();
    char other = (sym == 'X') ? 'O' : 'X';

    return count_sequences(sym) == count_sequences(other);
}

bool TTT5_Board::game_is_over(Player<char>* player) {
    return n_moves >= 24;
}

// ================== TTT5 UI ======================

TTT5_UI::TTT5_UI() : UI<char>(3) {
    cout << "\n--- Welcome to 5x5 Tic Tac Toe ---\n";
    cout << "Goal: At the end of the game (24 moves), whoever has more 3-in-a-row wins!\n";
}

Move<char>* TTT5_UI::get_move(Player<char>* player) {
    int x, y;

    // لو اللاعب كمبيوتر
    if (player->get_type() == PlayerType::COMPUTER) {
        do {
            x = rand() % player->get_board_ptr()->get_rows();
            y = rand() % player->get_board_ptr()->get_columns();
        } while (player->get_board_ptr()->get_cell(x, y) != '-'); 

        cout << "\n" << player->get_name() << " (Computer) played: (" << x << "," << y << ")\n";
        return new Move<char>(x, y, player->get_symbol());
    }

    // لو اللاعب إنسان:
    cout << player->get_name() << " (" << player->get_symbol() << ") enter (row col): ";
    cin >> x >> y;
    return new Move<char>(x, y, player->get_symbol());
}

