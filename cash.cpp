#include "cash.h"



cash::cash()
{
}

cash::cash(int money)
{
	type = "cash";
	amount = money;
}

void cash::print_trade()
{
	std::cout << "$" << amount;
}

std::string cash::get_name()
{
	return "";
}

int cash::get_cash_amount()
{
	return amount;
}

cash::~cash()
{
}
