#include "InfinityTTT.h"
#include <iostream>
using namespace std;

InfinityTTT::InfinityTTT() {
    move_count = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '-';
}

void InfinityTTT::printBoard() {
    cout << "\nBoard:\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << " ";
        cout << endl;
    }
}

bool InfinityTTT::checkWin(char s) {
    for (int i = 0; i < 3; i++)
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s)
            return true;

    for (int j = 0; j < 3; j++)
        if (board[0][j] == s && board[1][j] == s && board[2][j] == s)
            return true;

    if (board[0][0] == s && board[1][1] == s && board[2][2] == s)
        return true;

    if (board[0][2] == s && board[1][1] == s && board[2][0] == s)
        return true;

    return false;
}

void InfinityTTT::removeOldest() {
    if (move_history.size() < 3) return;

    auto oldest = move_history[0];
    move_history.erase(move_history.begin());
    board[oldest.first][oldest.second] = '-';
}

void InfinityTTT::play() {
    char player = 'X';

    while (true) {
        printBoard();
        cout << "Player " << player << " turn.\n";
        int r, c;
        
        cout << "Enter row & col (0-2): ";
        cin >> r >> c;

        if (r < 0 || r > 2 || c < 0 || c > 2 || board[r][c] != '-') {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        board[r][c] = player;
        move_history.push_back({r, c});
        move_count++;

        if (checkWin(player)) {
            printBoard();
            cout << "Player " << player << " wins!\n";
            break;
        }

        if (move_count % 3 == 0)
            removeOldest();

        player = (player == 'X') ? 'O' : 'X';
    }
}
