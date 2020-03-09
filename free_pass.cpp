#include "free_pass.h"
#include "Player.h"


free_pass::free_pass()
{
	owner = nullptr;
	turns = 0;
}

free_pass::free_pass(Player* owner_, int turn)
{
	type = "free_pass";
	owner = owner_;
	turns = turn;
}

void free_pass::print_trade()
{
	std::cout << turns << " free passes from " << owner->name;
}

free_pass::~free_pass()
{
}

std::string free_pass::get_name()
{
	return "";
}

int free_pass::get_cash_amount()
{
	return 0;
}
