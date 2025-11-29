#include "DiamondTTT.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

/* ---------------- DiamondTTT_Board ---------------- */
DiamondTTT_Board::DiamondTTT_Board() : Board<char>(5,5) {
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
            board[i][j]=' ';
    n_moves=0;
}
DiamondTTT_Board::~DiamondTTT_Board() {}

bool DiamondTTT_Board::update_board(Move<char>* move) {
    int r=move->get_x(), c=move->get_y();
    if(r<0 || r>=5 || c<0 || c>=5 || board[r][c]!=' ')
        return false;
    board[r][c]=move->get_symbol();
    n_moves++;
    return true;
}

bool DiamondTTT_Board::lines_in_different_directions(char mark) {
    return lines_in_different_directions_on_matrix(board, mark);
}

// FIXED: اللاعب يكسب لما هو يعمل خطين في اتجاهين مختلفين
bool DiamondTTT_Board::is_win(Player<char>* p) {
    char mark = p->get_symbol();
    return lines_in_different_directions(mark);
}

// FIXED: اللاعب يخسر لما الخصم يعمل خطين في اتجاهين مختلفين
bool DiamondTTT_Board::is_lose(Player<char>* p) {
    char mark = p->get_symbol();
    char opp = (mark=='X') ? 'O' : 'X';
    return lines_in_different_directions(opp);
}

bool DiamondTTT_Board::is_draw(Player<char>* p) {
    return n_moves>=25 && !lines_in_different_directions('X') && !lines_in_different_directions('O');
}

bool DiamondTTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

// Static helper for Smart AI
bool DiamondTTT_Board::lines_in_different_directions_on_matrix(const vector<vector<char>>& mat, char mark) {
    bool has3=false, has4=false;

    // صفوف (Rows)
    for(int i=0;i<5;i++){
        for(int j=0;j<=2;j++)
            if(mat[i][j]==mark && mat[i][j+1]==mark && mat[i][j+2]==mark) has3=true;
        for(int j=0;j<=1;j++)
            if(mat[i][j]==mark && mat[i][j+1]==mark && mat[i][j+2]==mark && mat[i][j+3]==mark) has4=true;
    }
    // أعمدة (Columns)
    for(int j=0;j<5;j++){
        for(int i=0;i<=2;i++)
            if(mat[i][j]==mark && mat[i+1][j]==mark && mat[i+2][j]==mark) has3=true;
        for(int i=0;i<=1;i++)
            if(mat[i][j]==mark && mat[i+1][j]==mark && mat[i+2][j]==mark && mat[i+3][j]==mark) has4=true;
    }
    // قطر رئيسي (\) Main Diagonal
    for(int i=0;i<=2;i++)
        for(int j=0;j<=2;j++){
            if(mat[i][j]==mark && mat[i+1][j+1]==mark && mat[i+2][j+2]==mark) has3=true;
            if(i<=1 && j<=1 && mat[i][j]==mark && mat[i+1][j+1]==mark && mat[i+2][j+2]==mark && mat[i+3][j+3]==mark) has4=true;
        }
    // قطر ثانوي (/) Anti-Diagonal
    for(int i=0;i<=2;i++)
        for(int j=2;j<=4;j++){
            if(mat[i][j]==mark && mat[i+1][j-1]==mark && mat[i+2][j-2]==mark) has3=true;
            if(i<=1 && j>=3 && mat[i][j]==mark && mat[i+1][j-1]==mark && mat[i+2][j-2]==mark && mat[i+3][j-3]==mark) has4=true;
        }

    return has3 && has4;
}

/* ---------------- DiamondTTT_Human ---------------- */
DiamondTTT_Human::DiamondTTT_Human(string name, char symbol)
    : Player<char>(name,symbol,PlayerType::HUMAN) {}

Move<char>* DiamondTTT_Human::get_move(Player<char>*) {
    int r,c;
    while(true){
        cout<<"Enter row and column for "<<symbol<<" (0-4): ";
        cin>>r>>c;
        if(r>=0 && r<5 && c>=0 && c<5){
            if(boardPtr && boardPtr->get_cell(r,c)==' ') break;
            else cout<<"Cell occupied! Try again.\n";
        } else cout<<"Invalid coordinates! Must be 0-4.\n";
    }
    return new Move<char>(r,c,symbol);
}

/* ---------------- DiamondTTT_Computer ---------------- */
DiamondTTT_Computer::DiamondTTT_Computer(string name,char symbol)
    : Player<char>(name,symbol,PlayerType::COMPUTER) {
    srand((unsigned)time(nullptr));
}

Move<char>* DiamondTTT_Computer::get_move(Player<char>*) {
    vector<vector<char>> mat(5,vector<char>(5,' '));
    if(boardPtr) mat = boardPtr->get_board_matrix();

    vector<pair<int,int>> empties;
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
            if(mat[i][j]==' ')
                empties.push_back({i,j});
    if(empties.empty()) return new Move<char>(0,0,symbol);

    bool useSmart = (get_name().find("[AI:Smart]")!=string::npos);

    if(!useSmart){
        int idx = rand()%empties.size();
        auto p = empties[idx];
        return new Move<char>(p.first,p.second,symbol);
    }

    // Smart AI: حاول الفوز أولاً
    vector<pair<int,int>> win_moves;
    for(auto &p: empties){
        mat[p.first][p.second]=symbol;
        if(DiamondTTT_Board::lines_in_different_directions_on_matrix(mat,symbol))
            win_moves.push_back(p);
        mat[p.first][p.second]=' ';
    }
    if(!win_moves.empty()){
        int idx = rand()%win_moves.size();
        auto p = win_moves[idx];
        return new Move<char>(p.first,p.second,symbol);
    }

    // منع خصم من الفوز
    char opp = (symbol=='X')?'O':'X';
    vector<pair<int,int>> block_moves;
    for(auto &p: empties){
        mat[p.first][p.second]=opp;
        if(DiamondTTT_Board::lines_in_different_directions_on_matrix(mat,opp))
            block_moves.push_back(p);
        mat[p.first][p.second]=' ';
    }
    if(!block_moves.empty()){
        int idx = rand()%block_moves.size();
        auto p = block_moves[idx];
        return new Move<char>(p.first,p.second,symbol);
    }

    // fallback random
    int idx = rand()%empties.size();
    auto p = empties[idx];
    return new Move<char>(p.first,p.second,symbol);
}

/* ---------------- DiamondTTT_UI ---------------- */
DiamondTTT_UI::DiamondTTT_UI() : UI("Diamond Tic Tac Toe", 1) {}

Move<char>* DiamondTTT_UI::get_move(Player<char>* p){
    if(auto human = dynamic_cast<DiamondTTT_Human*>(p))
        return human->get_move(nullptr);
    else if(auto comp = dynamic_cast<DiamondTTT_Computer*>(p))
        return comp->get_move(nullptr);
    else {
        cout<<"Unknown player type!\n";
        return nullptr;
    }
}

Player<char>* DiamondTTT_UI::create_player(string& name, char symbol, PlayerType type){
    if(type==PlayerType::HUMAN) return new DiamondTTT_Human(name,symbol);
    else{
        int ai_choice=1;
        cout<<"Choose AI for "<<name<<" (1=Random, 2=Smart): ";
        cin>>ai_choice;
        if(ai_choice==2) name+=" [AI:Smart]";
        else name+=" [AI:Random]";
        return new DiamondTTT_Computer(name,symbol);
    }
}