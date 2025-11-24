#ifndef SUS_UI_H
#define SUS_UI_H

#include "BoardGame_Classes.h"
#include "SUS_Classes.h"
using namespace std;

class SUS_UI : public UI<char> {
public:
    SUS_UI() : UI<char>("Welcome to SUS Game!", 3) {}

    Move<char>* get_move(Player<char>* p) override {
        int x, y;
        cout << p->get_name() << " (" << p->get_symbol() << ") enter row and column: ";
        cin >> x >> y;
        return new Move<char>(x, y, p->get_symbol());
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        return new Player<char>(name, symbol, type);
    }

    Player<char>** setup_players() override {
        Player<char>** players = new Player<char>*[2];

        string n1, n2;
        cout << "Enter Player 1 name: ";
        getline(cin >> ws, n1);

        cout << "Choose Player 1 symbol (S or U): ";
        char s1;
        cin >> s1;

        cout << "Enter Player 2 name: ";
        getline(cin >> ws, n2);

        cout << "Choose Player 2 symbol (S or U): ";
        char s2;
        cin >> s2;

        players[0] = new Player<char>(n1, s1, PlayerType::HUMAN);
        players[1] = new Player<char>(n2, s2, PlayerType::HUMAN);

        return players;
    }
};

#endif
