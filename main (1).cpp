#include <iostream>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_Classes.h"

using namespace std;

int main() {
    srand(time(0));

    while (true) {
        cout << "\n=== FCAI Board Games Menu ===\n";
        cout << "1. X-O Game\n";
        cout << "2. Numerical Tic Tac Toe\n";
        cout << "0. Exit\nChoose: ";

        int ch;
        cin >> ch;

        if (ch == 0) break;

        else if (ch == 1) {
            UI<char>* ui = new XO_UI();
            Board<char>* board = new X_O_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board;
            delete players[0];
            delete players[1];
            delete[] players;
        }

        else if (ch == 2) {
            UI<int>* ui = new Numerical_UI();
            Board<int>* board = new Numerical_Board();
            Player<int>** players = ui->setup_players();
            GameManager<int> gm(board, players, ui);
            gm.run();
            delete board;
            delete players[0];
            delete players[1];
            delete[] players;
        }
    }

    return 0;
}
