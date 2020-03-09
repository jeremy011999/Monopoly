#pragma once
#include "tradeable.h"
#include "card.h"

class chance :public card
{
public:
	chance();
	chance(std::string description_, int num_);
	std::string get_name();
	int get_cash_amount();
	~chance();
	void pulled();
	void print();
	int get_num();
	std::string description;
	int num;
};

