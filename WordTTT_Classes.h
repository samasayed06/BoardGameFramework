#ifndef WORDTTT_CLASSES_H
#define WORDTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <set>
#include <string>
#include <fstream>

class WordTTT_Board : public Board<char> {
private:
    std::set<std::string> dictionary;
    bool load_dictionary(const std::string& filename);
    bool check_word(int row, int col, int dr, int dc) const;
    bool has_valid_word() const;

public:
    WordTTT_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    
    // Helper methods for AI
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(char last_player) const;
    bool is_valid_dictionary_word(const std::string& word) const;
};

class WordTTT_UI : public UI<char> {
public:
    WordTTT_UI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
};

#endif // WORDTTT_CLASSES_H