#pragma once
#include "Square.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include "Player.h"
#include <algorithm>
class board
{
public:
	board();
	std::vector<Square*> squares;
	std::ifstream fin;
	void load();
	void print_titles();
	void print_board();
	void print_tokens();
	void print_info(int num);
	void next_player();
	void turn();
	int print_menu(const int check);
	int roll_dice();
	void trade();
	void buy_property();
	void send_to_jail();
	void print_trades();
	void passed_go();
	void print_cards();
	void draw_chance();
	void draw_community();
	void do_chance(int num);
	void do_cc(int num);
	void print_properties_left();
	void perform_trade();
	void start_auction();
	bool check_for_monopoly(Square* bought, Player* buying_player);
	void ai_turn();
	void check_values_for_trade();
	double find_property_value(Square* current, Player* POI);
	double num_of_turns_left();
	void average_property_probabilities();
	void start_ai_trade(Square* wanted, Player* owning_player);
	void ai_to_player_trade(Square* wanted, Player* owning_player);

	std::vector<std::string> game_info;
	std::map<int,std::pair<std::string,std::pair<int,int>>> Board;
	std::vector<Player*> players;
	std::vector<piece*> tokens;
	std::vector<int> chosen_tokens;
	std::vector<card*> chance_cards;
	std::vector<card*> community_chest_cards;
	Player* current_player;
	Player* trading_player;
	Player* highest_bidder;
	Square* current_square;
	int go_space = 0;
	int jail_space = 10;
	int free_parking_space = 20;
	int go_to_jail_space = 30;
	std::vector<tradeable*> to_be_traded, wanted;
	std::map<std::string, int> menu;
	std::map<std::string, int>::iterator menu_it;
	bool rolled_doubles;
	int doubles_counter;
	int shuffle_counter_chance=0, shuffle_counter_cc = 0;
	int available_houses = 32, available_hotels = 12;
	std::vector<Square*>* temp_monopoly_vector;
	std::vector<std::string> names={"Bill","Joe","Emily","Ann","Tim","Mark","Sarah"};

	~board();
};

/*
purple		80
cc
purple
INC TAX
RR				0
light blue		176
chance			240
light blue
light blue
jail			192
pink			208
utility
pink
pink
RR
orange			96
cc
orange
orange
free
red				64
chance
red
red
RR
yellow			224
yellow
utility
yellow
go to jail
green			32/160
green
cc
green
RR
chance
dark blue			144
LUX TAX				0
dark blue
GO


*/