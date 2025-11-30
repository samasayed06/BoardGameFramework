#include "PyramidTTT.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

/* ---------------- PyramidTTT_Board ---------------- */
PyramidTTT_Board::PyramidTTT_Board() : Board<char>(3,5) {
    // تهيئة البورد: الصف 0 فيه مربع واحد، الصف 1 فيه 3، الصف 2 فيه 5
    for(int i=0;i<3;i++)
        for(int j=0;j<5;j++)
            board[i][j]=' ';
    n_moves=0;
}

PyramidTTT_Board::~PyramidTTT_Board() {}

bool PyramidTTT_Board::is_valid_position(int r, int c) {
    // الصف 0: عمود 2 فقط (القمة)
    if(r==0) return c==2;
    // الصف 1: أعمدة 1, 2, 3 (الوسط)
    if(r==1) return c>=1 && c<=3;
    // الصف 2: أعمدة 0, 1, 2, 3, 4 (القاعدة)
    if(r==2) return c>=0 && c<=4;
    return false;
}

bool PyramidTTT_Board::update_board(Move<char>* move) {
    int r=move->get_x(), c=move->get_y();
    if(!is_valid_position(r,c) || board[r][c]!=' ')
        return false;
    board[r][c]=move->get_symbol();
    n_moves++;
    return true;
}

bool PyramidTTT_Board::has_three_in_a_row(char mark) {
    return has_three_in_a_row_on_matrix(board, mark);
}

// الفوز: 3 في صف واحد (أفقي، رأسي، قطري)
bool PyramidTTT_Board::is_win(Player<char>* p) {
    char mark = p->get_symbol();
    return has_three_in_a_row(mark);
}

bool PyramidTTT_Board::is_lose(Player<char>* p) {
    char mark = p->get_symbol();
    char opp = (mark=='X') ? 'O' : 'X';
    return has_three_in_a_row(opp);
}

bool PyramidTTT_Board::is_draw(Player<char>* p) {
    // إجمالي المربعات الصالحة: 1 + 3 + 5 = 9
    return n_moves>=9 && !has_three_in_a_row('X') && !has_three_in_a_row('O');
}

bool PyramidTTT_Board::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}

// Static helper للتحقق من 3 في صف
bool PyramidTTT_Board::has_three_in_a_row_on_matrix(const vector<vector<char>>& mat, char mark) {
    // صفوف أفقية
    // الصف 1: [1,1], [1,2], [1,3]
    if(mat[1][1]==mark && mat[1][2]==mark && mat[1][3]==mark) return true;
    // الصف 2: [2,0-1-2], [2,1-2-3], [2,2-3-4]
    if(mat[2][0]==mark && mat[2][1]==mark && mat[2][2]==mark) return true;
    if(mat[2][1]==mark && mat[2][2]==mark && mat[2][3]==mark) return true;
    if(mat[2][2]==mark && mat[2][3]==mark && mat[2][4]==mark) return true;

    // أعمدة رأسية
    // عمود 2: [0,2], [1,2], [2,2]
    if(mat[0][2]==mark && mat[1][2]==mark && mat[2][2]==mark) return true;
    // عمود 1: [1,1], [2,1]
    // عمود 3: [1,3], [2,3]
    // (لا يكفي 2 فقط، لذلك نتجاهلهم)

    // أقطار
    // قطر يسار: [0,2], [1,1], [2,0]
    if(mat[0][2]==mark && mat[1][1]==mark && mat[2][0]==mark) return true;
    // قطر يمين: [0,2], [1,3], [2,4]
    if(mat[0][2]==mark && mat[1][3]==mark && mat[2][4]==mark) return true;
    // أقطار أخرى في القاعدة
    // [2,0], [1,1], [0,2] - تم حسابه فوق
    // [2,2], [1,2], [0,2] - عمود رأسي تم حسابه
    // [2,4], [1,3], [0,2] - تم حسابه فوق

    return false;
}

/* ---------------- PyramidTTT_Human ---------------- */
PyramidTTT_Human::PyramidTTT_Human(string name, char symbol)
    : Player<char>(name,symbol,PlayerType::HUMAN) {}

Move<char>* PyramidTTT_Human::get_move(Player<char>*) {
    int r,c;
    while(true){
        cout<<"Enter row (0-2) and column (0-4) for "<<symbol<<": ";
        cin>>r>>c;
        
        // تحقق من صحة الموقع
        bool valid = false;
        if(r==0 && c==2) valid = true;
        else if(r==1 && c>=1 && c<=3) valid = true;
        else if(r==2 && c>=0 && c<=4) valid = true;
        
        if(valid){
            if(boardPtr && boardPtr->get_cell(r,c)==' ') break;
            else cout<<"Cell occupied! Try again.\n";
        } else {
            cout<<"Invalid position for pyramid shape!\n";
            cout<<"Valid positions:\n";
            cout<<"  Row 0: column 2 only\n";
            cout<<"  Row 1: columns 1-3\n";
            cout<<"  Row 2: columns 0-4\n";
        }
    }
    return new Move<char>(r,c,symbol);
}

/* ---------------- PyramidTTT_Computer ---------------- */
PyramidTTT_Computer::PyramidTTT_Computer(string name,char symbol)
    : Player<char>(name,symbol,PlayerType::COMPUTER) {
    srand((unsigned)time(nullptr));
}

Move<char>* PyramidTTT_Computer::get_move(Player<char>*) {
    vector<vector<char>> mat(3,vector<char>(5,' '));
    if(boardPtr) mat = boardPtr->get_board_matrix();

    vector<pair<int,int>> empties;
    // جمع المربعات الفارغة الصالحة فقط
    if(mat[0][2]==' ') empties.push_back({0,2});
    for(int c=1;c<=3;c++)
        if(mat[1][c]==' ') empties.push_back({1,c});
    for(int c=0;c<=4;c++)
        if(mat[2][c]==' ') empties.push_back({2,c});
    
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
        if(PyramidTTT_Board::has_three_in_a_row_on_matrix(mat,symbol))
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
        if(PyramidTTT_Board::has_three_in_a_row_on_matrix(mat,opp))
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

/* ---------------- PyramidTTT_UI ---------------- */
PyramidTTT_UI::PyramidTTT_UI() : UI("Pyramid Tic Tac Toe", 2) {}

Move<char>* PyramidTTT_UI::get_move(Player<char>* p){
    if(auto human = dynamic_cast<PyramidTTT_Human*>(p))
        return human->get_move(nullptr);
    else if(auto comp = dynamic_cast<PyramidTTT_Computer*>(p))
        return comp->get_move(nullptr);
    else {
        cout<<"Unknown player type!\n";
        return nullptr;
    }
}

Player<char>* PyramidTTT_UI::create_player(string& name, char symbol, PlayerType type){
    if(type==PlayerType::HUMAN) return new PyramidTTT_Human(name,symbol);
    else{
        int ai_choice=1;
        cout<<"Choose AI for "<<name<<" (1=Random, 2=Smart): ";
        cin>>ai_choice;
        if(ai_choice==2) name+=" [AI:Smart]";
        else name+=" [AI:Random]";
        return new PyramidTTT_Computer(name,symbol);
    }
}


void PyramidTTT_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    cout << "\n";
    cout << "          +---+\n";
    cout << "          | " << matrix[0][2] << " |\n";
    cout << "          +---+\n";
    cout << "      +---+---+---+\n";
    cout << "      | " << matrix[1][1] << " | " << matrix[1][2] << " | " << matrix[1][3] << " |\n";
    cout << "      +---+---+---+\n";
    cout << "  +---+---+---+---+---+\n";
    cout << "  | " << matrix[2][0] << " | " << matrix[2][1] << " | " << matrix[2][2] << " | " << matrix[2][3] << " | " << matrix[2][4] << " |\n";
    cout << "  +---+---+---+---+---+\n\n";
}