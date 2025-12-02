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
            board[i][j]='#';  // علامة للخلايا غير الصالحة
    
    // تحديد شكل الماسة - الخلايا الصالحة فقط
    // Row 0: column 2 only
    board[0][2] = ' ';
    // Row 1: columns 1,2,3
    board[1][1] = ' '; board[1][2] = ' '; board[1][3] = ' ';
    // Row 2: columns 0,1,2,3,4 (full row)
    board[2][0] = ' '; board[2][1] = ' '; board[2][2] = ' '; board[2][3] = ' '; board[2][4] = ' ';
    // Row 3: columns 1,2,3
    board[3][1] = ' '; board[3][2] = ' '; board[3][3] = ' ';
    // Row 4: column 2 only
    board[4][2] = ' ';
    
    n_moves=0;
}

DiamondTTT_Board::~DiamondTTT_Board() {}

bool DiamondTTT_Board::is_valid_cell(int r, int c) const {
    if(r<0 || r>=5 || c<0 || c>=5) return false;
    // شكل الماسة
    if(r==0 && c==2) return true;
    if(r==1 && (c==1||c==2||c==3)) return true;
    if(r==2) return true;  // الصف الأوسط كله صالح
    if(r==3 && (c==1||c==2||c==3)) return true;
    if(r==4 && c==2) return true;
    return false;
}

bool DiamondTTT_Board::update_board(Move<char>* move) {
    int r=move->get_x(), c=move->get_y();
    if(!is_valid_cell(r,c) || board[r][c]!=' ')
        return false;
    board[r][c]=move->get_symbol();
    n_moves++;
    return true;
}

bool DiamondTTT_Board::lines_in_different_directions(char mark) {
    return lines_in_different_directions_on_matrix(board, mark);
}

bool DiamondTTT_Board::is_win(Player<char>* p) {
    char mark = p->get_symbol();
    return lines_in_different_directions(mark);
}

bool DiamondTTT_Board::is_lose(Player<char>* p) {
    char mark = p->get_symbol();
    char opp = (mark=='X') ? 'O' : 'X';
    return lines_in_different_directions(opp);
}

bool DiamondTTT_Board::is_draw(Player<char>* p) {
    // الماسة تحتوي على 13 خلية صالحة (1+3+5+3+1)
    return n_moves>=13 && !lines_in_different_directions('X') && !lines_in_different_directions('O');
}

bool DiamondTTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

// Static helper for Smart AI
bool DiamondTTT_Board::lines_in_different_directions_on_matrix(const vector<vector<char>>& mat, char mark) {
    // Check for lines in each direction separately
    bool row3=false, row4=false;
    bool col3=false, col4=false;
    bool diag3=false, diag4=false;
    bool antidiag3=false, antidiag4=false;

    // Rows (Horizontal)
    for(int i=0;i<5;i++){
        for(int j=0;j<=2;j++)
            if(mat[i][j]==mark && mat[i][j+1]==mark && mat[i][j+2]==mark) row3=true;
        for(int j=0;j<=1;j++)
            if(mat[i][j]==mark && mat[i][j+1]==mark && mat[i][j+2]==mark && mat[i][j+3]==mark) row4=true;
    }
    
    // Columns (Vertical)
    for(int j=0;j<5;j++){
        for(int i=0;i<=2;i++)
            if(mat[i][j]==mark && mat[i+1][j]==mark && mat[i+2][j]==mark) col3=true;
        for(int i=0;i<=1;i++)
            if(mat[i][j]==mark && mat[i+1][j]==mark && mat[i+2][j]==mark && mat[i+3][j]==mark) col4=true;
    }
    
    // Main Diagonal (\)
    for(int i=0;i<=2;i++)
        for(int j=0;j<=2;j++){
            if(mat[i][j]==mark && mat[i+1][j+1]==mark && mat[i+2][j+2]==mark) diag3=true;
            if(i<=1 && j<=1 && mat[i][j]==mark && mat[i+1][j+1]==mark && mat[i+2][j+2]==mark && mat[i+3][j+3]==mark) diag4=true;
        }
    
    // Anti-Diagonal (/)
    for(int i=0;i<=2;i++)
        for(int j=2;j<=4;j++){
            if(mat[i][j]==mark && mat[i+1][j-1]==mark && mat[i+2][j-2]==mark) antidiag3=true;
            if(i<=1 && j>=3 && mat[i][j]==mark && mat[i+1][j-1]==mark && mat[i+2][j-2]==mark && mat[i+3][j-3]==mark) antidiag4=true;
        }

    // Check if we have line of 3 AND line of 4 in DIFFERENT directions
    // Horizontal direction
    if(row3 && (col4 || diag4 || antidiag4)) return true;
    if(row4 && (col3 || diag3 || antidiag3)) return true;
    
    // Vertical direction
    if(col3 && (row4 || diag4 || antidiag4)) return true;
    if(col4 && (row3 || diag3 || antidiag3)) return true;
    
    // Main Diagonal direction
    if(diag3 && (row4 || col4 || antidiag4)) return true;
    if(diag4 && (row3 || col3 || antidiag3)) return true;
    
    // Anti-Diagonal direction
    if(antidiag3 && (row4 || col4 || diag4)) return true;
    if(antidiag4 && (row3 || col3 || diag3)) return true;

    return false;
}

/* ---------------- DiamondTTT_Human ---------------- */
DiamondTTT_Human::DiamondTTT_Human(string name, char symbol)
    : Player<char>(name,symbol,PlayerType::HUMAN) {}

Move<char>* DiamondTTT_Human::get_move(Player<char>*) {
    int r,c;
    while(true){
        cout<<"\n>> Player "<<symbol<<"'s turn\n";
        cout<<"Example: To play center type: 2 2\n";
        cout<<"Enter row column: ";
        cin>>r>>c;
        
        if(r>=0 && r<5 && c>=0 && c<5){
            DiamondTTT_Board* diamondBoard = dynamic_cast<DiamondTTT_Board*>(boardPtr);
            if(diamondBoard && diamondBoard->is_valid_cell(r,c)){
                if(boardPtr->get_cell(r,c)==' ') break;
                else cout<<"[X] Cell occupied! Try another cell.\n";
            } else {
                cout<<"[X] This cell is not part of the diamond!\n";
                cout<<"Available cells:\n";
                cout<<"  Row 0: [0,2]\n";
                cout<<"  Row 1: [1,1] [1,2] [1,3]\n";
                cout<<"  Row 2: [2,0] [2,1] [2,2] [2,3] [2,4]\n";
                cout<<"  Row 3: [3,1] [3,2] [3,3]\n";
                cout<<"  Row 4: [4,2]\n";
            }
        } else cout<<"[X] Numbers must be between 0 and 4!\n";
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
    
    DiamondTTT_Board* diamondBoard = dynamic_cast<DiamondTTT_Board*>(boardPtr);

    vector<pair<int,int>> empties;
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
            if(diamondBoard && diamondBoard->is_valid_cell(i,j) && mat[i][j]==' ')
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

void DiamondTTT_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n";
    cout << "========================================\n";
    cout << "      Diamond Tic Tac Toe Game        \n";
    cout << "========================================\n\n";
    
    cout << "  Available cells shown with [row,col]\n";
    cout << "  Use (row column) to make your move\n\n";
    
    // Row 0 - Top of diamond
    cout << "            [0,2]\n";
    cout << "          +------+\n";
    cout << "          |  " << matrix[0][2] << "   |\n";
    cout << "          +------+\n\n";
    
    // Row 1
    cout << "      [1,1] [1,2] [1,3]\n";
    cout << "    +------+------+------+\n";
    cout << "    |  " << matrix[1][1] << "   |  " << matrix[1][2] << "   |  " << matrix[1][3] << "   |\n";
    cout << "    +------+------+------+\n\n";
    
    // Row 2 - Middle (widest)
    cout << "[2,0] [2,1] [2,2] [2,3] [2,4]\n";
    cout << "+------+------+------+------+------+\n";
    cout << "|  " << matrix[2][0] << "   |  " << matrix[2][1] << "   |  " << matrix[2][2] << "   |  " << matrix[2][3] << "   |  " << matrix[2][4] << "   |\n";
    cout << "+------+------+------+------+------+\n\n";
    
    // Row 3
    cout << "      [3,1] [3,2] [3,3]\n";
    cout << "    +------+------+------+\n";
    cout << "    |  " << matrix[3][1] << "   |  " << matrix[3][2] << "   |  " << matrix[3][3] << "   |\n";
    cout << "    +------+------+------+\n\n";
    
    // Row 4 - Bottom of diamond
    cout << "            [4,2]\n";
    cout << "          +------+\n";
    cout << "          |  " << matrix[4][2] << "   |\n";
    cout << "          +------+\n";
    
    cout << "\n========================================\n";
    cout << " WINNING CONDITION:\n";
    cout << " - Make a line of 3 marks AND\n";
    cout << " - Make a line of 4 marks\n";
    cout << " - Lines must be in DIFFERENT directions\n";
    cout << "   (one Horizontal, one Vertical, etc.)\n";
    cout << " - Lines can share ONE common mark\n";
    cout << "========================================\n" << endl;
}