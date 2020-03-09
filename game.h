#pragma once
#include "board.h"
class game
{
public:
	game();
	void start();
	void set_players();

	board* game_board;
	int current_turn;
	bool game_over;
	~game();
};

