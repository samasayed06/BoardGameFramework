#include "SUS_Classes.h"
using namespace std;

SUS_Board::SUS_Board()
    : Board<char>(3,3), scoreP1(0), scoreP2(0)
{
    for (int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            board[i][j] = '-';
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char s = move->get_symbol();

    if (x < 0 || x >= 3 || y < 0 || y >= 3)
        return false;

    if (board[x][y] != '-')
        return false;

    board[x][y] = s;
    n_moves++;

    // after placing symbol, count score again
    int sequences = count_SUS_sequences();

    if (s == 'S')
        scoreP1 = sequences;
    else
        scoreP2 = sequences;

    return true;
}

bool SUS_Board::is_win(Player<char>* p) {
    return false; // SUS has no instant win condition
}

bool SUS_Board::is_lose(Player<char>* p) {
    return false; // also none
}

bool SUS_Board::is_draw(Player<char>* p) {
    return n_moves == 9;
}

bool SUS_Board::game_is_over(Player<char>* p) {
    return is_draw(p);
}

// Count S-U-S patterns
int SUS_Board::count_SUS_sequences() {
    int count = 0;

    // rows
    for (int r=0;r<3;r++)
        if (board[r][0]=='S' && board[r][1]=='U' && board[r][2]=='S')
            count++;

    // cols
    for (int c=0;c<3;c++)
        if (board[0][c]=='S' && board[1][c]=='U' && board[2][c]=='S')
            count++;

    // diagonals
    if (board[0][0]=='S' && board[1][1]=='U' && board[2][2]=='S')
        count++;

    if (board[0][2]=='S' && board[1][1]=='U' && board[2][0]=='S')
        count++;

    return count;
}

int SUS_Board::getScore(char symbol) {
    if (symbol == 'S') return scoreP1;
    else return scoreP2;
}

void SUS_Board::print_scores(){
    cout << "\nScore (Player S): " << scoreP1
         << "     Score (Player U): " << scoreP2 << "\n";
}