#ifndef ULTIMATE_TTT_CLASSES_H
#define ULTIMATE_TTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

class UltimateTTT_Board : public Board<char> {
public:
	UltimateTTT_Board();

	
		bool update_board(Move<char>* move) override;
	bool is_win(Player<char>* player) override;
	bool is_lose(Player<char>* player) override;
	bool is_draw(Player<char>* player) override;
	bool game_is_over(Player<char>* player) override;

	std::pair<int, int> random_move(char symbol) const;
	std::pair<int, int> smart_move(char symbol) const;
	

private:
	std::vector<std::vector<char>> subWinner;   // 3×3 small-board winners
	int nextR, nextC; // forced next sub-board (-1 = free)

	
		bool in_bounds(int r, int c) const;
	char compute_sub_winner(int sr, int sc) const;
	bool main_winner(char s) const;

	std::vector<std::pair<int, int>> get_legal_moves() const;
	

};

class UltimateTTT_UI : public UI<char> {
public:
	UltimateTTT_UI();
	Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
	Move<char>* get_move(Player<char>* player) override;
};

#endif
