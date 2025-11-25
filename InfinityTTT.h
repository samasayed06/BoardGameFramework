#ifndef INFINITY_TTT_H
#define INFINITY_TTT_H

#include <iostream>
#include <vector>
using namespace std;

class InfinityTTT {
private:
    char board[3][3];
    vector<pair<int,int>> move_history; // حفظ ترتيب الحركات
    int move_count;

public:
    InfinityTTT();
    void printBoard();
    bool checkWin(char symbol);
    void removeOldest();
    void play();
};

#endif
