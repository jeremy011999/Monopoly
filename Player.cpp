#include "Player.h"



Player::Player(bool player)
{
	is_player = player;
	money = new cash(1500);
	//if (!is_player)
	//{
	//	money->amount = 20000;
	//}
	token = new piece(1);
	has_made_first_pass = false;
	possible_positions.insert(std::pair<int, int>(0, 2279));
	for (int i = 1; i < 10; i++)
	{
		possible_positions.insert(std::pair<int, int>(i, (32 - 3 * i) * 69 + 2));
	}
	possible_positions.insert(std::pair<int, int>(10, 71));
	for (int i = 11; i < 20; i++)
	{
		possible_positions.insert(std::pair<int, int>(i, 79 + (6 * (i-11))));
	}
	possible_positions.insert(std::pair<int, int>(20, 134));
	for (int i = 21; i < 30; i++)
	{
		possible_positions.insert(std::pair<int, int>(i, 410+3*(69*(i-21))));
	}
	possible_positions.insert(std::pair<int, int>(30, 2342));
	for (int i = 31; i < 40; i++)
	{
		possible_positions.insert(std::pair<int, int>(i, 2335 - ((i - 31) * 6)));
	}
}

/*
77
377
602
827
1052
1277
1502
1727
1952
2177
2402
2702

32
29
26
*/

void Player::print_properties()
{
	for (int i = 0; i < properties_owned.size(); i++)
	{
		properties_owned[i]->print();
	}
	system("PAUSE");
}

int Player::print_prop_names(std::vector<tradeable*> v)
{
	int x=50;
	int selection = 1;
	std::cout << name << "'s Properties: \n";
	for (int i = 0; i < properties_owned.size(); i++)
	{
		for (int j = 0; j < v.size(); j++)
		{
			if (v[j] == properties_owned[i])
			{
				x = j;
				goto after;
			}
		}
		std::cout << selection << ". " << properties_owned[i]->name << std::endl;
		selection++;
	after:;
	}
	return x;
}

void Player::buy_houses()
{
monopoly_selection:
	std::cout << "Which monopoly would you like to buy houses for?\n";
	print_monopolies();
	std::string input = "empty";
	std::cin >> input;
	if (input[0]<'0' || input[0]>'9')
	{
		input = "0";
	}
	while (std::stoi(input) < 1 && std::stoi(input) > monopolies_vector.size()+1)
	{
		std::cout << "Which monopoly would you like to buy houses for?\n";
		std::cin >> input;
		if (input[0]<'0' || input[0]>'9')
		{
			input = "0";
		}
	}
	std::cout << "Are you sure? (Y/N)\n";
	int selection = std::stoi(input);
	if (selection == monopolies_vector.size() + 1)
	{
		return;
	}
	selection--;
	input = "";
	std::cin >> input;
	while (input != "Y" && input != "y" && input != "N" && input != "n")
	{
		std::cout << "You inputted " << input << std::endl;
		std::cin.clear();
		std::cout << "Are you sure? (Y/N)\n";
		std::cin >> input;
	}
	if (input == "Y" || input == "y")
	{
		std::vector<Square*> temp_houses, temp_hotels;
		bool has_one_available = false;
		int fewest_houses = 10;
		for (size_t i = 0; i < monopolies_vector[selection]->size(); i++)
		{
			if ((*monopolies_vector[selection])[i]->num_houses < 4)
			{
				has_one_available = true;
				temp_houses.push_back((*monopolies_vector[selection])[i]);
				if ((*monopolies_vector[selection])[i]->num_houses < fewest_houses)
				{
					fewest_houses = (*monopolies_vector[selection])[i]->num_houses;
				}
			}
			else if ((*monopolies_vector[selection])[i]->num_hotels == 0 && (*monopolies_vector[selection])[i]->num_houses == 4)
			{
				temp_hotels.push_back((*monopolies_vector[selection])[i]);
			}
		}
		if (temp_houses.size() > 0)
		{
		house_property_selection:
			std::cout << "Which Property would you like to buy houses for?\n";
			for (size_t i = 0; i < temp_houses.size(); i++)
			{
				int display = i + 1;
				if (temp_houses[i]->num_houses > fewest_houses)
				{
					display--;
				}
				if (temp_houses[i]->num_houses <= fewest_houses)
				{
					std::cout << display << ". " << temp_houses[i]->name <<" ("<<temp_houses[i]->price_per_house<<")"<< std::endl;
				}
				if (i == temp_houses.size() - 1)
				{
					std::cout << display+1 << ". Go Back\n";
				}
			}
			std::string house_choice = "empty";
			std::cin >> house_choice;
			while (std::stoi(house_choice) < 1 && std::stoi(house_choice) > temp_houses.size()+1)
			{
				std::cout << "Which property would you like to buy houses for?\n";
				std::cin >> house_choice;
			}
			if (std::stoi(house_choice) == temp_houses.size() + 1)
			{
				goto monopoly_selection;
			}
			std::cout << "Are you sure? (Y/N)\n";
			int choice = std::stoi(house_choice);
			choice--;
			std::cin >> house_choice;
			while (house_choice != "Y" && house_choice != "y" && house_choice != "N" && house_choice != "n")
			{
				std::cout << "Are you sure? (Y/N)\n";
				std::cin >> house_choice;
			}
			if (house_choice == "Y" || house_choice == "y")
			{
				temp_houses[choice]->num_houses++;
				money->amount -= temp_houses[choice]->price_per_house;
				switch (temp_houses[choice]->num_houses)
				{
				case 1:
				{
					temp_houses[choice]->current_rent = temp_houses[choice]->rent_1h;
					break;
				}
				case 2:
				{
					temp_houses[choice]->current_rent = temp_houses[choice]->rent_2h;
					break;
				}
				case 3:
				{
					temp_houses[choice]->current_rent = temp_houses[choice]->rent_3h;
					break;
				}
				case 4:
				{
					temp_houses[choice]->current_rent = temp_houses[choice]->rent_4h;
					break;
				}
				}
			}
			if (house_choice == "N" || house_choice == "n")
			{
				goto house_property_selection;
			}
		}
		else if (temp_hotels.size() >0)
		{
		hotel_property_selection:
			std::cout << "Which Property would you like to buy hotels for?\n";
			for (size_t i = 0; i < temp_hotels.size(); i++)
			{
				std::cout << i + 1 << ". " << temp_hotels[i]->name <<" ("<<temp_hotels[i]->price_per_house<<")"<< std::endl;
				if (i == temp_hotels.size() - 1)
				{
					std::cout << i + 2 << ". Go Back\n";
				}
			}
			std::string house_choice = "empty";
			std::cin >> house_choice;
			while (std::stoi(house_choice) < 1 && std::stoi(house_choice) > temp_hotels.size() + 1)
			{
				std::cout << "Which property would you like to buy houses for?\n";
				std::cin >> house_choice;
			}
			if (std::stoi(house_choice) == temp_hotels.size() + 1)
			{
				goto monopoly_selection;
			}
			std::cout << "Are you sure? (Y/N)\n";
			int choice = std::stoi(house_choice);
			choice--;
			std::cin >> house_choice;
			while (house_choice != "Y" && house_choice != "y" && house_choice != "N" && house_choice != "n")
			{
				std::cout << "Are you sure? (Y/N)\n";
				std::cin >> house_choice;
			}
			if (house_choice == "Y" || house_choice == "y")
			{
				temp_hotels[choice]->num_hotels++;
				temp_hotels[choice]->current_rent = temp_hotels[choice]->rent_hotel;
				money->amount -= temp_hotels[choice]->price_per_house;
			}
			if (house_choice == "N" || house_choice == "n")
			{
				goto hotel_property_selection;
			}
		}

	}
}

void Player::print_monopolies()
{
	for (int i = 0; i < monopolies_vector.size(); i++)
	{
		if ((*monopolies_vector[i])[0]->color == "RailRoad" || (*monopolies_vector[i])[0]->color == "Utility")
		{
			continue;
		}
		std::cout << i + 1 << ". " << (*monopolies_vector[i])[0]->color<<" (";
		for (int j = 0; j < monopolies_vector[i]->size(); j++)
		{
			std::cout << (*monopolies_vector[i])[j]->name;
			if (j < monopolies_vector[i]->size() - 1)
			{
				std::cout << "/";
			}
		}
		std::cout << ")\n";
		if (i == monopolies_vector.size() - 1)
		{
			std::cout << i + 2 << ". Go Back\n";
		}
	}
}

double Player::get_wealth()
{
	double sum=0;
	sum += money->amount;
	for (int i = 0; i < properties_owned.size(); i++)
	{
		sum += properties_owned[i]->price;
	}
	sum += (num_of_GOJ * 50);
	return sum;
}


Player::~Player()
{
}
