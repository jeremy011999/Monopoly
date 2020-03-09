#pragma once
#include "piece.h"
#include "cash.h"
#include "Square.h"
#include "free_pass.h"
#include "chance.h"
#include "community_chest.h"
#include <map>
#include <vector>
class Player
{
public:
	Player(bool player);
	
	int PIN=0;
	int jail_turn_counter = 0;
	bool is_player;
	bool is_in_jail;
	bool can_go;
	int num_of_GOJ;
	piece* token;
	cash* money;
	int number;
	int houses, hotels, monopolies;
	std::vector<Square*> properties_owned;
	std::map<int, int>::iterator current_position;
	std::map<int, int> possible_positions;
	std::string name;
	Square* current_location;
	std::vector<free_pass*> passes;
	card* get_out_card;
	bool has_made_first_pass;
	std::vector<std::vector<Square*>*> monopolies_vector;
	int purp_owned = 0, lb_owned = 0, pink_owned = 0, oj_owned = 0, red_owned = 0, yellow_owned = 0, green_owned = 0, blue_owned = 0, util_owned = 0, rr_owned = 0;
	std::map<double, Square*> values;
	std::pair<double, Square*> most_valuable_property;

	void print_properties();
	int print_prop_names(std::vector<tradeable*> v);
	void buy_houses();
	void print_monopolies();
	double get_wealth();

	~Player();
};

