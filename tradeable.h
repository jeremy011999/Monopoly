#pragma once
#include <iostream>
#include <string>
#include <vector>

class Player;
class tradeable
{
public:
	tradeable();
	virtual ~tradeable();
	virtual void print_trade();
	std::string type;
	Player* owner;
	virtual std::string get_name() = 0;
	virtual int get_cash_amount()=0;
};

