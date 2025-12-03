#ifndef MEMORYTTT_CLASSES_H
#define MEMORYTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

class MemoryTTT_Board : public Board<char> {
private:
    std::vector<std::vector<char>> hidden_board; // اللوحة الحقيقية المخفية
    
    bool check_three_in_row(char sym) const;

public:
    MemoryTTT_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // للحصول على اللوحة المخفية (للتحقق من الفوز)
    std::vector<std::vector<char>> get_hidden_board() const { return hidden_board; }
    
    // للذكاء الاصطناعي
    std::pair<int, int> random_move() const;
    std::pair<int, int> smart_move(char symbol);
};

class MemoryTTT_UI : public UI<char> {
public:
    MemoryTTT_UI();
    Move<char>* get_move(Player<char>* player) override;
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    
    // عرض اللوحة المخفية (كلها فارغة ظاهرياً)
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
};

#endif