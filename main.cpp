#include <iostream>
#include <ctime>
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_Classes.h"
#include "misere_Classes.h"
#include "SUS_Classes.h"
#include "TTT5_Classes.h"
#include "Obstacles_Classes.h"
#include "InfinityTTT_Classes.h"
#include "TicTacToe4x4.h"
#include "DiamondTTT.h"
#include "PyramidTTT.h"

using namespace std;

int main() {
    srand(time(0));

    while (true) {
        cout << "\n=== FCAI Board Games Menu ===\n";
        cout << "1. X-O Game\n";
        cout << "2. Numerical Tic Tac Toe\n";
        cout << "3. Misère Tic Tac Toe (Inverse XO)\n";
        cout << "4. SUS Game\n";
        cout << "5. 5x5 Tic Tac Toe\n";
        cout << "6. Obstacles Tic Tac Toe (6x6)\n";
        cout << "7. Infinity Tic Tac Toe\n";   
        cout << "8. 4x4 Tic Tac Toe\n";
        cout << "9. Diamond Tic Tac Toe\n";
        cout << "10. Pyramid Tic Tac Toe\n";
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
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 2) {
            UI<int>* ui = new Numerical_UI();
            Board<int>* board = new Numerical_Board();
            Player<int>** players = ui->setup_players();
            GameManager<int> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 3) {
            UI<char>* ui = new Misere_UI();
            Board<char>* board = new Misere_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 4) {
            UI<char>* ui = new SUS_UI();
            Board<char>* board = new SUS_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 5) {
            UI<char>* ui = new TTT5_UI();
            Board<char>* board = new TTT5_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 6) {
            UI<char>* ui = new Obstacles_UI();
            Board<char>* board = new Obstacles_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 7) {
            UI<char>* ui = new InfinityTTT_UI();
            Board<char>* board = new InfinityTTT_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 8) {
            UI<char>* ui = new TicTacToe4x4_UI<char>();
            Board<char>* board = new TicTacToe4x4_Board<char>();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 9) {
            UI<char>* ui = new DiamondTTT_UI();
            Board<char>* board = new DiamondTTT_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else if (ch == 10) {
            UI<char>* ui = new PyramidTTT_UI();
            Board<char>* board = new PyramidTTT_Board();
            Player<char>** players = ui->setup_players();
            GameManager<char> gm(board, players, ui);
            gm.run();
            delete board; delete players[0]; delete players[1]; delete[] players;
        }

        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}