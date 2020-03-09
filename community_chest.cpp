#include "community_chest.h"



community_chest::community_chest()
{
}

community_chest::community_chest(std::string description_, int num_) : description(description_)
{
	type = "community_chest";
	num = num_;
}

std::string community_chest::get_name()
{
	return "";
}

int community_chest::get_cash_amount()
{
	return 0;
}

void community_chest::print()
{
	std::cout << description << std::endl;
}

int community_chest::get_num()
{
	return num;
}

community_chest::~community_chest()
{
}

void community_chest::pulled()
{
	switch (num)
	{
	case 0:
	{
		
	}
	case 1:
	{

	}
	case 2:
	{

	}
	case 3:
	{

	}
	case 4:
	{

	}
	case 5:
	{

	}
	case 6:
	{

	}
	case 7:
	{

	}
	case 8:
	{

	}
	case 9:
	{

	}
	case 10:
	{

	}
	case 11:
	{

	}
	case 12:
	{

	}
	case 13:
	{

	}
	case 14:
	{

	}
	case 15:
	{

	}
	case 16:
	{

	}
	}
}