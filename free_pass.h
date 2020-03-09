#pragma once
#include "tradeable.h"
class Player;

class free_pass :
	public tradeable
{
public:
	free_pass(Player* owner_, int turns_);
	free_pass();
	int turns;

	std::string get_name();
	int get_cash_amount();
	void print_trade();
	~free_pass();
};

