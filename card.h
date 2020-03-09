#pragma once
#include "tradeable.h"
class card :public tradeable
{
public:
	card();
	card(std::string description_, int num_);
	virtual ~card();
	virtual void print() = 0;
	void print_trade();

	std::string description;
	virtual int get_num() = 0;
	virtual void pulled();
};

