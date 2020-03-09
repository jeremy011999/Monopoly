#include "Square.h"
#include "Player.h"


Square::Square()
{

}

Square::Square(std::string name_, int number, int price_, int price_per_house_, int rent_, int rent_1, int rent_2, int rent_3, int rent_4, int rent_h, int mortgage_)
{
	type = "square";
	owner = nullptr;
	name = name_;
	num = number;
	rent = rent_;
	price = price_;
	price_per_house = price_per_house_;
	rent_1h = rent_1;
	rent_2h = rent_2;
	rent_3h = rent_3;
	rent_4h = rent_4;
	rent_hotel = rent_h;
	mortgage = mortgage_;
	current_rent = rent;
	switch (num)
	{
	case 0:
	{
		is_go_square = true; break;
	}
	case 2:
	{
		is_c_chest_square = true; break;
	}
	case 4:
	{
		is_income_tax_square = true; break;
	}
	case 5:
	{
		is_railroad_square = true; break;
	}
	case 7:
	{
		is_chance_square = true; break;
	}
	case 10:
	{
		is_jail_square = true; break;
	}
	case 12:
	{
		is_utility_square = true; break;
	}
	case 15:
	{
		is_railroad_square = true; break;
	}
	case 17:
	{
		is_c_chest_square = true; break;
	}
	case 20:
	{
		is_free_parking_square = true; break;
	}
	case 22:
	{
		is_chance_square = true; break;
	}
	case 25:
	{
		is_railroad_square = true; break;
	}
	{
	case 28:
	{
		is_utility_square = true; break;
	}
	case 30:
	{
		is_send_to_jail_square = true; break;
	}
	case 33:
	{
		is_c_chest_square = true; break;
	}
	case 35:
	{
		is_railroad_square = true; break;
	}
	case 36:
	{
		is_chance_square = true; break;
	}
	case 38:
	{
		is_luxury_tax_square = true; break;
	}
	}
	}
	num_hotels = num_houses = 0;
	is_owned = false;
	if (price > 0)
	{
		can_be_owned = true;
	}
	if (can_be_owned && (is_utility_square || is_railroad_square))
	{
		can_have_houses=false;
	}
}

void Square::print()
{
	std::cout << "Name: "<<name << std::endl;
	std::cout << "Property Number: "<< num << std::endl;
	std::cout << "Price: "<< price << std::endl;
	std::cout <<"Cost of a house: "<< price_per_house << std::endl;
	std::cout << "Rent: " << rent << std::endl;
	std::cout << "Rent with 1 house: "<<rent_1h << std::endl;
	std::cout << "Rent with 2 houses: "<<rent_2h << std::endl;
	std::cout << "Rent with 3 houses: "<<rent_3h << std::endl;
	std::cout << "Rent with 4 houses: "<<rent_4h << std::endl;
	std::cout << "Rent with a hotel: "<<rent_hotel << std::endl;
	std::cout << "Current Rent: " << current_rent << std::endl;
	std::cout << "Mortgage value: "<<mortgage << std::endl;
	std::cout << std::endl;
}

void Square::print_trade()
{
	std::cout << name << " from " << owner->name << " worth " << price << std::endl;
}

void Square::print_name()
{
	std::cout << name << std::endl;
}

std::string Square::get_name()
{
	return name;
}

int Square::get_cash_amount()
{
	return 0;
}

Square::~Square()
{

}
