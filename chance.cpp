#include "chance.h"



chance::chance()
{
}

chance::chance(std::string description_, int num_) : description(description_)
{
	type = "chance";
	num = num_;
}

std::string chance::get_name()
{
	return "";
}

int chance::get_cash_amount()
{
	return 0;
}

chance::~chance()
{
}

void chance::print()
{
	std::cout << description << std::endl;
}

int chance::get_num()
{
	return num;
}

void chance::pulled()
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
	}
}