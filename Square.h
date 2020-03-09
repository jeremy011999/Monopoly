#pragma once
#include <string>
#include <iostream>
#include "tradeable.h"

class Player;

class Square : public tradeable
{
public:
	Square();
	Square(std::string name_, int number, int price_, int price_per_house_, int rent_, int rent_1, int rent_2, int rent_3, int rent_4, int rent_h, int mortgage_);
	std::string name, color;
	int num;
	int current_rent;
	int color_num;
	bool is_chance_square = false;
	bool is_c_chest_square = false;
	bool is_jail_square = false;
	bool is_send_to_jail_square = false;
	bool is_go_square = false;
	bool is_railroad_square = false;
	bool is_utility_square = false;
	bool is_luxury_tax_square = false;
	bool is_income_tax_square = false;
	bool can_have_houses = true;
	int price, price_per_house;
	int rent, rent_1h, rent_2h, rent_3h, rent_4h, rent_hotel, mortgage;
	bool is_owned = false;
	bool can_be_owned = false;
	int num_houses;
	int num_hotels;
	bool is_free_parking_square;
	int row_num;
	int symbol_num, house_num, letter_num;
	bool is_in_a_monopoly = false;
	std::vector<double> probabilities;
	
	std::string get_name();
	int get_cash_amount();
	void print();
	void print_trade();
	void print_name();

	~Square();
};

