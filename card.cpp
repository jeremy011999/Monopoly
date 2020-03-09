#include "card.h"



card::card()
{
}

card::card(std::string description_,int num_) : description(description_)
{
	
}


card::~card()
{
}

void card::print_trade()
{
	std::cout << this->description;
}

void card::pulled()
{
	
}