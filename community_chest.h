#pragma once
#include "tradeable.h"
#include "card.h"
class community_chest :public card
{
public:
	community_chest();
	community_chest(std::string description_, int num_);
	~community_chest();
	void pulled();
	void print();
	int get_num();
	std::string description;
	std::string get_name();
	int get_cash_amount();
	int num;
};

