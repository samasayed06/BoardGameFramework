#include "misere_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;

/* ---------------- Misere_Board ---------------- */
// كل الدوال موجودة في الهيدر

/* ---------------- Misere_Human ---------------- */
Misere_Human::Misere_Human(string name, char symbol) : Player<char>(name, symbol, PlayerType::HUMAN) {}
Move<char>* Misere_Human::get_move(Player<char>*) {
    int r,c;
    cout << "Enter row and col for " << symbol << ": ";
    cin >> r >> c;
    return new Move<char>(r,c,symbol);
}

/* ---------------- Misere_Computer ---------------- */
Misere_Computer::Misere_Computer(string name, char symbol) : Player<char>(name, symbol, PlayerType::COMPUTER) {
    srand(time(nullptr));
}
Move<char>* Misere_Computer::get_move(Player<char>*) {
    int r,c;
    do {
        r = rand()%3;
        c = rand()%3;
    } while (boardPtr && boardPtr->get_board_matrix()[r][c] != ' ');
    return new Move<char>(r,c,symbol);
}

/* ---------------- Misere_UI ---------------- */
Misere_UI::Misere_UI() : UI("Misère Tic Tac Toe (Inverse XO)",1) {}
Move<char>* Misere_UI::get_move(Player<char>* p) {
    if(auto human = dynamic_cast<Misere_Human*>(p))
        return human->get_move(nullptr);
    else if(auto comp = dynamic_cast<Misere_Computer*>(p))
        return comp->get_move(nullptr);
    else {
        cout << "Unknown player type!" << endl;
        return nullptr;
    }
}
Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    if(type == PlayerType::HUMAN) return new Misere_Human(name,symbol);
    else return new Misere_Computer(name,symbol);
}
