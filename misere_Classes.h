#ifndef _MISERE_TTT_CLASSES_H
#define _MISERE_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

/* ---------------- Misere Board ---------------- */
class Misere_Board : public Board<char> {
public:
    Misere_Board() : Board<char>(3,3) {
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
                board[i][j] = ' ';
    }

    virtual ~Misere_Board() {}

    bool update_board(Move<char>* move) override {
        int r = move->get_x();
        int c = move->get_y();
        if (r<0 || r>=3 || c<0 || c>=3 || board[r][c]!=' ') return false;
        board[r][c] = move->get_symbol();
        n_moves++;
        return true;
    }

    bool is_win(Player<char>* p) override {
        char mark = p->get_symbol();
        char opponent = (mark=='X')?'O':'X';
        return has_three_in_row(opponent);
    }

    bool is_lose(Player<char>* p) override {
        char mark = p->get_symbol();
        return has_three_in_row(mark);
    }

    bool is_draw(Player<char>* p) override {
        return n_moves>=9 && !has_three_in_row('X') && !has_three_in_row('O');
    }

    bool game_is_over(Player<char>* p) override {
        return is_lose(p) || is_win(p) || is_draw(p);
    }

private:
    bool has_three_in_row(char mark) {
        for(int i=0;i<3;i++)
            if(board[i][0]==mark && board[i][1]==mark && board[i][2]==mark) return true;
        for(int j=0;j<3;j++)
            if(board[0][j]==mark && board[1][j]==mark && board[2][j]==mark) return true;
        if(board[0][0]==mark && board[1][1]==mark && board[2][2]==mark) return true;
        if(board[0][2]==mark && board[1][1]==mark && board[2][0]==mark) return true;
        return false;
    }
};

/* ---------------- Misere Human Player ---------------- */
class Misere_Human : public Player<char> {
public:
    Misere_Human(string name, char symbol) : Player<char>(name, symbol, PlayerType::HUMAN) {}
    Move<char>* get_move(Player<char>*) {
        int r,c;
        cout<<"Enter row and col for "<<symbol<<": ";
        cin>>r>>c;
        return new Move<char>(r,c,symbol);
    }
};

/* ---------------- Misere Computer Player ---------------- */
class Misere_Computer : public Player<char> {
public:
    Misere_Computer(string name, char symbol) : Player<char>(name, symbol, PlayerType::COMPUTER) {
        srand(time(nullptr));
    }
    Move<char>* get_move(Player<char>*) {
        int r,c;
        do {
            r = rand()%3;
            c = rand()%3;
        } while (boardPtr && boardPtr->get_board_matrix()[r][c] != ' ');
        return new Move<char>(r,c,symbol);
    }
};

/* ---------------- Misere UI ---------------- */
class Misere_UI : public UI<char> {
public:
    Misere_UI(): UI("Misère Tic Tac Toe (Inverse XO)",1) {}

    Move<char>* get_move(Player<char>* p) override {
        // استدعاء الحركة على حسب نوع اللاعب
        if(auto human = dynamic_cast<Misere_Human*>(p))
            return human->get_move(nullptr);
        else if(auto comp = dynamic_cast<Misere_Computer*>(p))
            return comp->get_move(nullptr);
        else {
            cout << "Unknown player type!" << endl;
            return nullptr;
        }
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        if(type==PlayerType::HUMAN) return new Misere_Human(name,symbol);
        else return new Misere_Computer(name,symbol);
    }
};

#endif
#ifndef _MISERE_TTT_CLASSES_H
#define _MISERE_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

/* ---------------- Misere Board ---------------- */
class Misere_Board : public Board<char> {
public:
    Misere_Board() : Board<char>(3,3) {
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
                board[i][j] = ' ';
    }

    virtual ~Misere_Board() {}

    bool update_board(Move<char>* move) override {
        int r = move->get_x();
        int c = move->get_y();
        if (r<0 || r>=3 || c<0 || c>=3 || board[r][c]!=' ') return false;
        board[r][c] = move->get_symbol();
        n_moves++;
        return true;
    }

    bool is_win(Player<char>* p) override {
        char mark = p->get_symbol();
        char opponent = (mark=='X')?'O':'X';
        return has_three_in_row(opponent);
    }

    bool is_lose(Player<char>* p) override {
        char mark = p->get_symbol();
        return has_three_in_row(mark);
    }

    bool is_draw(Player<char>* p) override {
        return n_moves>=9 && !has_three_in_row('X') && !has_three_in_row('O');
    }

    bool game_is_over(Player<char>* p) override {
        return is_lose(p) || is_win(p) || is_draw(p);
    }

private:
    bool has_three_in_row(char mark) {
        for(int i=0;i<3;i++)
            if(board[i][0]==mark && board[i][1]==mark && board[i][2]==mark) return true;
        for(int j=0;j<3;j++)
            if(board[0][j]==mark && board[1][j]==mark && board[2][j]==mark) return true;
        if(board[0][0]==mark && board[1][1]==mark && board[2][2]==mark) return true;
        if(board[0][2]==mark && board[1][1]==mark && board[2][0]==mark) return true;
        return false;
    }
};

/* ---------------- Misere Human Player ---------------- */
class Misere_Human : public Player<char> {
public:
    Misere_Human(string name, char symbol) : Player<char>(name, symbol, PlayerType::HUMAN) {}
    Move<char>* get_move(Player<char>*) {
        int r,c;
        cout<<"Enter row and col for "<<symbol<<": ";
        cin>>r>>c;
        return new Move<char>(r,c,symbol);
    }
};

/* ---------------- Misere Computer Player ---------------- */
class Misere_Computer : public Player<char> {
public:
    Misere_Computer(string name, char symbol) : Player<char>(name, symbol, PlayerType::COMPUTER) {
        srand(time(nullptr));
    }
    Move<char>* get_move(Player<char>*) {
        int r,c;
        do {
            r = rand()%3;
            c = rand()%3;
        } while (boardPtr && boardPtr->get_board_matrix()[r][c] != ' ');
        return new Move<char>(r,c,symbol);
    }
};

/* ---------------- Misere UI ---------------- */
class Misere_UI : public UI<char> {
public:
    Misere_UI(): UI("Misère Tic Tac Toe (Inverse XO)",1) {}

    Move<char>* get_move(Player<char>* p) override {
        // استدعاء الحركة على حسب نوع اللاعب
        if(auto human = dynamic_cast<Misere_Human*>(p))
            return human->get_move(nullptr);
        else if(auto comp = dynamic_cast<Misere_Computer*>(p))
            return comp->get_move(nullptr);
        else {
            cout << "Unknown player type!" << endl;
            return nullptr;
        }
    }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override {
        if(type==PlayerType::HUMAN) return new Misere_Human(name,symbol);
        else return new Misere_Computer(name,symbol);
    }
};

#endif
