#include "WordTTT_Classes.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using namespace std;

// ================== WordTTT_Board Implementation ======================

WordTTT_Board::WordTTT_Board() : Board<char>(3, 3) {
    // Initialize empty board
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = '-';
        }
    }
    
    n_moves = 0;
    
    // Load dictionary from file
    if (!load_dictionary("dic.txt")) {
        cout << "Warning: Could not load dic.txt. Using default words.\n";
        // Add some default 3-letter words if file not found
        dictionary = {"CAT", "DOG", "BAT", "RAT", "HAT", "MAT", 
                     "CAR", "BAR", "TAR", "ACE", "AGE", "ATE",
                     "EAT", "TEA", "ARE", "EAR", "ERA"};
    }
    
    cout << "Dictionary loaded with " << dictionary.size() << " words.\n";
}

bool WordTTT_Board::load_dictionary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    string word;
    while (file >> word) {
        // Convert to uppercase and store only 3-letter words
        if (word.length() == 3) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            dictionary.insert(word);
        }
    }
    
    file.close();
    return !dictionary.empty();
}

bool WordTTT_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();
    
    // Validate position
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        cout << "Invalid position. Row and column must be 0-2.\n";
        return false;
    }
    
    // Check if cell is empty
    if (board[x][y] != '-') {
        cout << "Cell is not empty.\n";
        return false;
    }
    
    // Validate that symbol is a letter
    if (!isalpha(sym)) {
        cout << "Invalid input. Please enter a letter (A-Z).\n";
        return false;
    }
    
    // Convert to uppercase
    sym = toupper(sym);
    
    // Place the letter
    board[x][y] = sym;
    n_moves++;
    
    return true;
}

bool WordTTT_Board::check_word(int row, int col, int dr, int dc) const {
    string word = "";
    
    // Build the word in the given direction
    for (int i = 0; i < 3; ++i) {
        int r = row + i * dr;
        int c = col + i * dc;
        
        // Check bounds
        if (r < 0 || r >= rows || c < 0 || c >= columns) {
            return false;
        }
        
        // Check if cell is empty
        if (board[r][c] == '-') {
            return false;
        }
        
        word += board[r][c];
    }
    
    // Check if word exists in dictionary
    return dictionary.find(word) != dictionary.end();
}

bool WordTTT_Board::has_valid_word() const {
    // Check all rows (horizontal)
    for (int i = 0; i < rows; ++i) {
        if (check_word(i, 0, 0, 1)) {
            return true;
        }
    }
    
    // Check all columns (vertical)
    for (int j = 0; j < columns; ++j) {
        if (check_word(0, j, 1, 0)) {
            return true;
        }
    }
    
    // Check main diagonal (top-left to bottom-right)
    if (check_word(0, 0, 1, 1)) {
        return true;
    }
    
    // Check anti-diagonal (top-right to bottom-left)
    if (check_word(0, 2, 1, -1)) {
        return true;
    }
    
    return false;
}

bool WordTTT_Board::is_win(Player<char>* player) {
    // The player who just moved wins if a valid word is formed
    return has_valid_word();
}

bool WordTTT_Board::is_lose(Player<char>* player) {
    return false;
}

bool WordTTT_Board::is_draw(Player<char>* player) {
    // Check if board is full
    bool full = true;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == '-') {
                full = false;
                break;
            }
        }
        if (!full) break;
    }
    
    // Draw if board is full and no valid word exists
    return full && !has_valid_word();
}

bool WordTTT_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool WordTTT_Board::is_valid_dictionary_word(const string& word) const {
    string upper_word = word;
    transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);
    return dictionary.find(upper_word) != dictionary.end();
}

pair<int, int> WordTTT_Board::random_move() const {
    vector<pair<int, int>> empty_cells;
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == '-') {
                empty_cells.push_back({i, j});
            }
        }
    }
    
    if (empty_cells.empty()) {
        return {-1, -1};
    }
    
    return empty_cells[rand() % empty_cells.size()];
}

pair<int, int> WordTTT_Board::smart_move(char last_player) const {
    // Try to complete a word or block opponent
    
    // First, try to complete a winning word
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] != '-') continue;
            
            // Try each letter A-Z
            for (char c = 'A'; c <= 'Z'; ++c) {
                const_cast<char&>(board[i][j]) = c;
                bool wins = has_valid_word();
                const_cast<char&>(board[i][j]) = '-';
                
                if (wins) {
                    return {i, j};
                }
            }
        }
    }
    
    // If can't win immediately, play randomly
    return random_move();
}

// ================== WordTTT_UI Implementation ======================

WordTTT_UI::WordTTT_UI() : UI<char>(3) {
    cout << "\n========================================\n";
    cout << "       Word Tic-Tac-Toe Game\n";
    cout << "========================================\n";
    cout << "Players place letters on a 3x3 board to form valid 3-letter words.\n";
    cout << "First to form a valid word (horizontally, vertically, or diagonally) wins!\n";
    cout << "The game is a draw if the board fills without forming a valid word.\n";
    cout << "========================================\n\n";
}

Player<char>* WordTTT_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::COMPUTER) {
        int ai;
        cout << "Choose AI level for " << name << ":\n";
        cout << "  1. Random AI\n";
        cout << "  2. Smart AI\n";
        cout << "Your choice: ";
        cin >> ai;
        
        if (ai == 2) {
            name += " [AI:Smart]";
        } else {
            name += " [AI:Random]";
        }
    }
    
    return new Player<char>(name, symbol, type);
}

Move<char>* WordTTT_UI::get_move(Player<char>* player) {
    int x, y;
    char letter;
    
    if (player->get_type() == PlayerType::COMPUTER) {
        Board<char>* bptr = player->get_board_ptr();
        WordTTT_Board* wb = dynamic_cast<WordTTT_Board*>(bptr);
        pair<int, int> mv = {-1, -1};
        
        if (wb) {
            string nm = player->get_name();
            if (nm.find("[AI:Smart]") != string::npos) {
                mv = wb->smart_move(player->get_symbol());
            } else {
                mv = wb->random_move();
            }
        }
        
        if (mv.first == -1) {
            mv = wb ? wb->random_move() : make_pair(0, 0);
        }
        
        // Random letter for computer
        letter = 'A' + (rand() % 26);
        
        cout << "\n" << player->get_name()
             << " (Computer) played: Row " << mv.first 
             << ", Col " << mv.second 
             << ", Letter '" << letter << "'\n";
        
        return new Move<char>(mv.first, mv.second, letter);
    }
    
    // Human player
    cout << "\n" << player->get_name() << "'s turn\n";
    cout << "Enter your move (row col letter): ";
    cin >> x >> y >> letter;
    
    return new Move<char>(x, y, letter);
}