#pragma once
#include "tradeable.h"
class cash :
	public tradeable
{
public:
	cash();
	cash(int money);
	int amount;

	std::string get_name();
	int get_cash_amount();
	void print_trade();
	~cash();
};

