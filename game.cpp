#include "game.h"



game::game()
{
	game_board = new board();
	current_turn = 1;
}

void game::start()
{
	game_board->load();
	set_players();
	while (!game_over)
	{
		game_board->turn();
	}
}

void game::set_players()
{
	for (int i = 0; i < game_board->players.size(); i++)
	{
		game_board->players[i]->current_position = game_board->players[i]->possible_positions.begin();
	}
}

game::~game()
{
}
