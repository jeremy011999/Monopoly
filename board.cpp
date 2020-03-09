#include "board.h"
#include <cstdlib>
#include <iomanip>
#include <stdlib.h>
#include <conio.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const int LIGHT_BLUE = 187;
const int PURPLE = 85;
const int PINK = 221;
const int ORANGE = 102;
const int RED = 204;
const int YELLOW = 238;
const int GREEN = 170;
const int BLUE = 153;
const int UTILITY = 119;
const int RAILROAD = 136;

board::board()
{
	std::srand(time(NULL));

	std::string name;
	int price, price_per_house, rent, rent_1h, rent_2h, rent_3h, rent_4h, rent_hotel, mortgage;
	fin.open("names.txt");
	if (fin.fail())
	{
		std::cout << "Could not open names file\n";
	}
	for (int i = 0; i < 40; i++)
	{
		fin >> name;
		fin >> price;
		fin >> price_per_house;
		fin >> rent;
		fin >> rent_1h;
		fin >> rent_2h;
		fin >> rent_3h;
		fin >> rent_4h;
		fin >> rent_hotel;
		fin >> mortgage;
		//fin nums for pieces when owned.
		squares.push_back(new Square(name, i, price, price_per_house, rent, rent_1h, rent_2h, rent_3h, rent_4h, rent_hotel, mortgage));
	}
	fin.close();
	fin.open("probabilities.txt");
	if (fin.fail())
	{
		std::cout << "could not open probabilities file\n";
	}
	std::string probability;
	int i = 0;
	while (fin>>probability)
	{
		if (probability == "Short" || probability == "Long")
		{
			continue;
		}
		while (probability[0] > 'A'&&probability[0] < 'z')
		{
			if (i == 40)
			{
				i = 0;
			}
			if (probability == squares[i]->name)
			{
				break;
			}
			else
			{
				i++;
			}
		}
		if (probability[0] >='0'&&probability[0]<='9')
		{
			squares[i]->probabilities.push_back(std::stod(probability));
		}
		
	}
	
	average_property_probabilities();
	
	fin.close();
	for (int i = 0; i < squares.size(); i++)
	{
		for (int j = 0; j < squares[i]->name.size(); j++)
		{
			if (squares[i]->name[j] == '_')
			{
				squares[i]->name[j] = ' ';
			}
		}
	}
	for (int i = 0; i < squares.size(); i++)
	{
		if (i > 0 && i < 5)		//purple
		{
			squares[i]->color_num = 85;
			squares[i]->color = "Purple";
		}
		else if (i == 5||i==15||i==25||i==35)		//railroad
		{
			squares[i]->color_num = 136;
			squares[i]->color = "RailRoad";
		}
		else if (i > 5 && i < 10)		//lb
		{
			squares[i]->color_num = 187;
			squares[i]->color = "Light Blue";
		}
		else if (i > 10 && i < 15)		//pink
		{
			squares[i]->color_num = 221;
			squares[i]->color = "Pink";
		}
		else if (i > 15 && i < 20)		//orange
		{
			squares[i]->color_num = 102;
			squares[i]->color = "Orange";
		}
		else if (i > 20 && i < 25)		//red
		{
			squares[i]->color_num = 204;
			squares[i]->color = "Red";
		}
		else if (i > 25 && i < 30)		//yellow
		{
			squares[i]->color_num = 238;
			squares[i]->color = "Yellow";
		}
		else if (i > 30 && i < 35)		//green
		{
			squares[i]->color_num = 170;
			squares[i]->color = "Green";
		}
		else if (i > 35 && i < 40)		//blue
		{
			squares[i]->color_num = 153;
			squares[i]->color = "Blue";
		}
		if (i == 28 || i == 12)		//utilities
		{
			squares[i]->color_num = 119;
			squares[i]->color = "Utility";
		}
	}
	std::string description="a";
	fin.open("chance.txt");
	if (fin.fail())
		std::cout << "Could not open chance file\n";
	int number = 0;
	while (1)
	{
		std::getline(fin, description);
		if (description == "stop")
			break;
		chance_cards.push_back(new chance(description, number));
		number++;
	}
	std::random_shuffle(chance_cards.begin(), chance_cards.end());

	fin.close();
	description = "a";
	fin.open("community_chest.txt");
	if (fin.fail())
		std::cout << "Could not open community chest file\n";
	number = 0;
	while (1)
	{
		std::getline(fin, description);
		if (description == "stop")
			break;
		community_chest_cards.push_back(new community_chest(description, number));
		number++;
	}
	std::random_shuffle(community_chest_cards.begin(), community_chest_cards.end());
	fin.close();
}

void board::next_player()
{
	bool player_was_found = false;
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->number == current_player->number + 1)
		{
			current_player = players[i];
			player_was_found = true;
			break;
		}
	}
	if (!player_was_found)
	{
		current_player = players[0];
	}
}

void board::load()
{
	for (int i = 0; i < 5; i++)
	{
		tokens.push_back(new piece(i+1));
	}
	for (int i = 0; i < 2415; i++)
	{
		if (i % 69 < 7 || i % 69 > 61) //side bits
		{
			Board[i] = std::make_pair("_", std::make_pair(0,0));
			continue;
		}
		if (i < 280 || i>2135) //top and bottom bits
		{
			Board[i] = std::make_pair("|", std::make_pair(0,0));
			continue;
		}
		
		else
		{
			Board[i] = std::make_pair("_", std::make_pair(0,0));
		}
	}

	for (int i = 1; i < 10; i++)
	{
		int step = 69;
		int base = 2008;
		squares[i]->symbol_num = base - (((i - 1) * 3) * step);
		Board[squares[i]->symbol_num].second.first = 0;
		squares[i]->house_num = squares[i]->symbol_num - step;
		squares[i]->letter_num = squares[i]->symbol_num + step;
		Board[squares[i]->house_num].second.first = 0;
		Board[squares[i]->letter_num].second.first = 0;
		Board[squares[i]->symbol_num].second.second = 2;
		Board[squares[i]->house_num].second.second = 1;
		Board[squares[i]->letter_num].second.second = 3;
	}
	for (int i = 29; i > 20; i--)
	{
		int step = 69;
		int base = 2062;
		squares[i]->symbol_num = base + (((i - 29) * 3) * step);
		Board[squares[i]->symbol_num].first = "a";
		Board[squares[i]->symbol_num].second.first = 0;
		squares[i]->house_num = squares[i]->symbol_num - step;
		squares[i]->letter_num = squares[i]->symbol_num + step;
		Board[squares[i]->house_num].second.first = 0;
		Board[squares[i]->letter_num].second.first = 0;
		Board[squares[i]->symbol_num].second.second = 2;
		Board[squares[i]->house_num].second.second = 1;
		Board[squares[i]->letter_num].second.second = 3;
	}
	for (int i = 11; i < 20; i++)
	{
		int jump = 6;
		int step = 2;
		int base = 217;
		squares[i]->symbol_num = base + (((i - 11)) * jump);
		Board[squares[i]->symbol_num].first = "a";
		Board[squares[i]->symbol_num].second.first = 0;
		squares[i]->house_num = squares[i]->symbol_num - step;
		squares[i]->letter_num = squares[i]->symbol_num + step;
		Board[squares[i]->house_num].second.first = 0;
		Board[squares[i]->letter_num].second.first = 0;
		Board[squares[i]->symbol_num].second.second = 2;
		Board[squares[i]->house_num].second.second = 1;
		Board[squares[i]->letter_num].second.second = 3;
	}
	for (int i = 39; i >30; i--)
	{
		int jump = 6;
		int step = 2;
		int base = 2149;
		squares[i]->symbol_num = base - (((i - 39)) * jump);
		Board[squares[i]->symbol_num].first = "a";
		Board[squares[i]->symbol_num].second.first = 0;
		squares[i]->house_num = squares[i]->symbol_num - step;
		squares[i]->letter_num = squares[i]->symbol_num + step;
		Board[squares[i]->house_num].second.first = 0;
		Board[squares[i]->letter_num].second.first = 0;
		Board[squares[i]->symbol_num].second.second = 2;
		Board[squares[i]->house_num].second.second = 1;
		Board[squares[i]->letter_num].second.second = 3;
	}

	Board[0].first= "J"; Board[1].first = "A"; Board[2].first = "I"; Board[3].first = "L";
	Board[68].first = "E"; Board[67].first = "E"; Board[66].first = "R"; Board[65].first = "F";
	Board[2346].first = "G"; Board[2347].first = "O";
	Board[2414].first = "J"; Board[2413].first = "2"; Board[2412].first = "G";
	int num_players= 0, num_ai= 0;
back:
	std::cout << "Welcome to Monopoly!\nHow many players? (Up to 5)\n";
	std::cin >> num_players;
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Try Again!\n";
		std::cin >> num_players;
	}
	while (num_players > 5)
	{
		std::cout << "Try Again!\n";
		std::cin >> num_players;
	}
	if (num_players != 5)
	{
		std::cout << "How many AI players? (Up to "<<5-num_players<<")\n";
		std::cin >> num_ai;
	}
	while (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Try Again!\n";
		std::cin >> num_ai;
	}
	while ((num_players + num_ai) > 5)
	{
		std::cout << "Try again!\n";
		std::cin >> num_ai;
	}
	for (int i = 0; i < num_players; i++)
	{
		players.push_back(new Player(true));		//actual player
	}
	for (int i = 0; i < num_ai; i++)
	{
		players.push_back(new Player(false));		//ai
	}
	std::vector<std::string>::iterator iter;
	for (int j = 0; j < players.size(); j++)
	{
		players[j]->number = j + 1;
		players[j]->current_location = squares[0];
		if (!players[j]->is_player)
		{
			players[j]->name = names[rand() % names.size()];
			for (iter = names.begin(); iter != names.end(); ++iter)
			{
				if (*iter == players[j]->name)
				{
					names.erase(iter);
					iter = names.begin();
				}
			}
		}
	}
	int token_counter = num_players; int i = 1; std::string token_choice = "";
	for (int i = 0; i < num_players; i++)
	{
		std::string name_;
		std::cout << "Player " << i + 1 << " please enter your name.\n";
		if (i == 0)
		{
			std::cin.ignore();
		}
		std::getline(std::cin, name_);
		players[i]->name = name_;
		players[i]->number = i+1;
		players[i]->current_location = squares[0];
	}
	for (int i = 0; i < num_players; i++)
	{
		std::string pin_ = "";
		char ch;
	nope:
		std::cout << "Player " << i + 1 << " please enter a 4-digit PIN for trades.\n";
		ch = _getch();
		while (ch != 13)
		{
			if (ch >= 48&&ch<=57)
			{
				pin_.push_back(ch);
				std::cout << '*';
				_ungetch(1);
			}
			ch = _getch();
			if ((ch == 0||ch==13||ch==1)&& pin_.size()>=4)
			{
				std::cin.clear();
				break;
			}
		}
		std::cin.clear();
		if ((pin_ >= "0000"&&pin_ <= "9999") && pin_.size() == 4)
		{
			players[i]->PIN = std::stoi(pin_);
		}
		else
			goto nope;
		std::cin.ignore();
	}
	while (token_counter > 0)
	{
again:
		std::cout << "Player " << i << " please choose a token:\n\n";
		for (int j = 1; j < tokens.size() + 1; j++)
		{
			bool found = false;
			std::cout << j << ". ";
			for (int k = 0; k < chosen_tokens.size(); k++)
			{
				if (chosen_tokens[k] == j)
				{
					found = true;		//piece has already been found
				}
			}
			if (!found)
			{
				tokens[j - 1]->print();
			}
			std::cout << std::endl;
		}
		std::cin >> token_choice;
		if (token_choice == "1" || token_choice == "2" || token_choice == "3" || token_choice == "4" || token_choice == "5")
		{
			players[i - 1]->token->piece_type = tokens[std::stoi(token_choice)-1]->piece_type;
			i++;
			token_counter--;
			chosen_tokens.push_back(std::stoi(token_choice));
		}
		else
		{
			goto again;
		}
	}
	
	int n = 0;
	while (chosen_tokens.size() != players.size())
	{
		
		
		choose_again:
			bool taken = false;
			std::srand(std::time(NULL));
			int random = std::rand() % 5+1;
			for (int i = 0; i < chosen_tokens.size(); i++)
			{
				if (random == chosen_tokens[i])
				{
					taken = true;
				}
			}
			if (!taken)
			{
				players[players.size() - num_ai + n]->token->piece_type = tokens[random-1]->piece_type;
				n++;
				chosen_tokens.push_back(random);
			}
			else
			{
				goto choose_again;
			}
	}
	current_player = players[0];
	print_tokens();
	system("PAUSE");
}

void board::print_tokens()
{
	for (int i = 0; i < players.size(); i++)
	{
		std::cout << "Player " << i + 1 << ": ";
		players[i]->token->print();
		std::cout << std::endl;
	}
}

void board::print_titles()
{
	for (int i = 0; i < squares.size(); i++)
	{
		squares[i]->print();
	}
}

void board::print_board()
{
	system("CLS");
	SetConsoleTextAttribute(hConsole, 0);
	std::cout << std::endl;
	bool need_to_print_symbols = false;
	for (int i = 0; i < 35; i++)
	{
		if (i - 3 <= 0||i+4>=35)
			SetConsoleTextAttribute(hConsole, 255);
		else
			SetConsoleTextAttribute(hConsole, 0);
		for (int j = 0; j < 69; j++)
		{
			need_to_print_symbols = false;
			int actual_num = i * 69 + j;
			if (j - 7 <= 0 || j + 8 >= 69)	//making white
			{
				SetConsoleTextAttribute(hConsole, 255);
			}
			else if (i - 3 > 0 && i + 4 < 35)	//turns middle black
			{
				SetConsoleTextAttribute(hConsole, 0);
			}
			else	//making white again
			{
				SetConsoleTextAttribute(hConsole, 255);
			}
			if ((i == 3 || i == 30)&&(j<=7||j>=61)) //making border show for top and bottom row
			{
				SetConsoleTextAttribute(hConsole, 240);
			}
			if (j == 7)		//left side
			{
				if (i > 3 && i < 10)
				{
					SetConsoleTextAttribute(hConsole, 187);		//lb
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 176);
						need_to_print_symbols = true;
					}
				}
				else if (i > 12 && i < 16)
				{
					SetConsoleTextAttribute(hConsole, 187);		//lb
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 176);
						need_to_print_symbols = true;
					}
				}
				else if (i > 21 && i < 25)
				{
					SetConsoleTextAttribute(hConsole, 85);		//purple
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 95);
						need_to_print_symbols = true;
					}
				}
				else if (i > 27 && i < 31)
				{
					SetConsoleTextAttribute(hConsole, 85);		//purple
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 95);
						need_to_print_symbols = true;
					}
				}
				else if (i > 24 && i < 28)
				{
					SetConsoleTextAttribute(hConsole, 34);		//cc
				}
				else if (i > 18 && i < 22)
				{
					SetConsoleTextAttribute(hConsole, 68);		//inc tax
				}
				else if (i > 9 && i < 13)
				{
					SetConsoleTextAttribute(hConsole, 51);		//chance
				}
			}
			else if (i == 3)		//top
			{
				if (j >= 44 && j <= 49)
				{
					SetConsoleTextAttribute(hConsole, 34);		//cc
				}
				else if (j >= 8 && j <= 12)
				{
					SetConsoleTextAttribute(hConsole, 221);		//pink
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 208);
						need_to_print_symbols = true;
					}
				}
				else if (j > 18 && j < 33)
				{
					SetConsoleTextAttribute(hConsole, 221);		//pink
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 208);
						need_to_print_symbols = true;
					}
				}
				else if (j >= 38 && j <= 43)
				{
					SetConsoleTextAttribute(hConsole, 102);		//orange
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 96);
						need_to_print_symbols = true;
					}
				}
				else if (j >= 50 && j <= 61)
				{
					SetConsoleTextAttribute(hConsole, 102);		//orange
					if (Board[actual_num].second.first > 0)
					{
						SetConsoleTextAttribute(hConsole, 96);
						need_to_print_symbols = true;
					}
				}
			}
			if (i == 31 && ((j >= 44 && j <= 49)))		//Green properties
			{
				SetConsoleTextAttribute(hConsole, 34); 
			}
			if (i == 31 && ((j >= 14 && j <= 18)))		//luxury tax
			{
				SetConsoleTextAttribute(hConsole, 68);
			}
			if (j == 61 && ((i > 18 && i < 25)||(i>27&&i<31)))		//yellow properties
			{
				SetConsoleTextAttribute(hConsole, 238);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 224);
					need_to_print_symbols = true;
				}
			}
			if (j == 61 && ((i > 3 && i < 7) || (i >8  && i < 16)))		//red properties
			{
				SetConsoleTextAttribute(hConsole, 204);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 192);
					need_to_print_symbols = true;
				}
			}
			if (j == 61 && i > 24 && i < 28)				//water works
			{
				SetConsoleTextAttribute(hConsole, 119);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 112);
					need_to_print_symbols = true;
				}
			}
			if ((j >= 13 && j <= 18) && i == 3)		//electric company
			{
				SetConsoleTextAttribute(hConsole, 119);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 112);
					need_to_print_symbols = true;
				}
			}
			if (i > 3 && i < 30 && (i) % 3 == 0 && (j <= 6 || j >= 62))		//making horizontal lines between spaces on sides
			{
				SetConsoleTextAttribute(hConsole, 240);
			}
			if (i==31&&((j >= 8 && j <= 12)||(j>19&&j<25)))		//dark blue properties
			{
				SetConsoleTextAttribute(hConsole, 153);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 159);
					need_to_print_symbols = true;
				}
			}
			if (i == 31 && ((j >= 38 && j <= 42) || (j >= 50 && j <= 60)))		//Green properties
			{
				SetConsoleTextAttribute(hConsole, 170);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 160);
					need_to_print_symbols = true;
				}
			}
			if (i == 31 && (j > 24 && j < 31))		//top and bottom chances
			{
				SetConsoleTextAttribute(hConsole, 51);
			}
			if (j == 61 && (i < 10 && i>6))		//side chances
			{
				SetConsoleTextAttribute(hConsole, 51);
			}
			if ((i == 3 || i == 31) && (j > 30 && j < 37))		//top and bottom railroads
			{
				SetConsoleTextAttribute(hConsole, 136);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 128);
					need_to_print_symbols = true;
				}
			}
			if ((i > 15 && i < 19) && (j == 7 || j == 61))		//left and right railroads
			{
				SetConsoleTextAttribute(hConsole, 136);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 128);
					need_to_print_symbols = true;
				}
			}

			if ((j >= 7 && j <= 61) && (j - 1) % 6 == 0 && (i < 4 || i>30))		//making vertical borders between properties on top and bottom rows
			{
				SetConsoleTextAttribute(hConsole, 240);
				if (Board[actual_num].second.first > 0)
				{
					SetConsoleTextAttribute(hConsole, 32);
					need_to_print_symbols = true;
				}
			}
			if (i == 0 && (j >= 0 && j <= 3))		//jail
			{
				SetConsoleTextAttribute(hConsole, 252);
			}
			int counter = 0;
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->current_position->second == actual_num)
				{
					if (counter >= 1)
						break;
					counter++;
					SetConsoleTextAttribute(hConsole, 240);
					players[i]->token->print();
				}
			}
			if (actual_num <=68&&actual_num>=65)		//free parking
			{
				SetConsoleTextAttribute(hConsole, 250);
			}
			if (actual_num == 2346 || actual_num == 2347)		//g2j
			{
				SetConsoleTextAttribute(hConsole, 250);
			}
			if (actual_num >= 2412 && actual_num <= 2414)		//go
			{
				SetConsoleTextAttribute(hConsole, 252);
			}
			
			//printing the actual thing to the console
			if (counter == 0)
			{
				if (need_to_print_symbols)
				{
					if (Board[actual_num].second.second == 1 && squares[Board[actual_num].second.first]->is_in_a_monopoly)
					{
						if (!squares[Board[actual_num].second.first]->can_have_houses)
						{
							squares[Board[actual_num].second.first]->owner->token->print();
						}
						else if (squares[Board[actual_num].second.first]->num_hotels < 1)
						{
							std::cout << squares[Board[actual_num].second.first]->num_houses;
						}
						else
						{
							std::cout << squares[Board[actual_num].second.first]->num_hotels;
						}
					}
					else if (Board[actual_num].second.second == 2)
					{
						if (squares[Board[actual_num].second.first]->is_owned)
						{
							squares[Board[actual_num].second.first]->owner->token->print();
						}
					}
					else if (Board[actual_num].second.second == 3 && squares[Board[actual_num].second.first]->is_in_a_monopoly)
					{
						if (!squares[Board[actual_num].second.first]->can_have_houses)
						{
							squares[Board[actual_num].second.first]->owner->token->print();
						}
						else if (squares[Board[actual_num].second.first]->num_hotels < 1)
						{
							std::cout << "h";
						}
						else
						{
							std::cout << "H";
						}
					}
					else if (!squares[Board[actual_num].second.first]->is_in_a_monopoly)
					{
						SetConsoleTextAttribute(hConsole, squares[Board[actual_num].second.first]->color_num);
						std::cout << Board[actual_num].first;
					}
				}
				else
				{
					std::cout << Board[actual_num].first;
				}
			}


			SetConsoleTextAttribute(hConsole, 0);

			if (i == 2 && j == 68)
			{
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "\tKEY:";
				SetConsoleTextAttribute(hConsole, 15);
				std::cout<<"    Chance ";
				SetConsoleTextAttribute(hConsole, 51);
				std::cout << "____";
				SetConsoleTextAttribute(hConsole, 15);
				std::cout << "  CC ";
				SetConsoleTextAttribute(hConsole, 34);
				std::cout << "____";
				SetConsoleTextAttribute(hConsole, 15);
				std::cout << "  RR ";
				SetConsoleTextAttribute(hConsole, 136);
				std::cout << "____";
				SetConsoleTextAttribute(hConsole, 15);
				std::cout << "  Tax ";
				SetConsoleTextAttribute(hConsole, 68);
				std::cout << "____";
				SetConsoleTextAttribute(hConsole, 15);
				std::cout << "  Util ";
				SetConsoleTextAttribute(hConsole, 119);
				std::cout << "____";
			}

		}
		SetConsoleTextAttribute(hConsole, 0);
		std::cout << "I";
		if (i > 7 && i < (14 + current_player->properties_owned.size()+players.size()))
		{
			if (i > 9 && i < 9 + current_player->properties_owned.size())
			{

			}
			SetConsoleTextAttribute(hConsole, 15);
			std::cout<<"\t\t";												//HEREISH I THINK
			print_info(i - 8);
		}
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
}

int board::print_menu(const int check)
{
	std::string s;
	menu.clear();
	if (players.size() == 1)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Move_Forward\n";
		menu.insert(std::pair<std::string, int>(s, 0));
	}
	if (current_player->can_go)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Roll Dice\n";
		menu.insert(std::pair<std::string, int>(s, 1));
	}
	if (!current_player->current_location->is_owned&&current_player->current_location->can_be_owned)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Buy Property ($";
		s+= std::to_string(current_player->current_location->price);
		s += ")\n";
		menu.insert(std::pair<std::string, int>(s, 7));
	}
	s = "";
	s += std::to_string(menu.size() + 1);
	s += ". See Properties\n";
	menu.insert(std::pair<std::string, int>(s, 2));
	if (!current_player->is_in_jail)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Initiate Trade\n";
		menu.insert(std::pair<std::string, int>(s, 3));
	}
	if (current_player->is_in_jail)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Pay $50 to get out of Jail\n";
		menu.insert(std::pair<std::string, int>(s, 4));
	}
	if (current_player->is_in_jail&&current_player->num_of_GOJ>0)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Use Get out of Jail Free Card\n";
		menu.insert(std::pair<std::string, int>(s, 5));
	}
	bool none_left = true;
	for (int i = 0; i < squares.size(); i++)
	{
		if (squares[i]->is_owned)
		{
			none_left = false;
		}
	}
	if (!none_left)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". See Properties Left\n";
		menu.insert(std::pair<std::string, int>(s, 6));
	}
	if (current_player->monopolies > 0)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". Buy Houses/Hotels\n";
		menu.insert(std::pair<std::string, int>(s, 10));
	}
	if (!current_player->can_go)
	{
		s = "";
		s += std::to_string(menu.size() + 1);
		s += ". End Turn\n";
		menu.insert(std::pair<std::string, int>(s, 9));
	}
	if (check > 0)
	{
		return menu.size();
	}
	for (menu_it=menu.begin();menu_it!=menu.end();menu_it++)
	{
		std::cout << menu_it->first;
	}

	return menu.size();
}

void board::turn()
{
	if (!current_player->is_player)
	{
		ai_turn();
		next_player();
		return;
	}
	rolled_doubles = false;
	doubles_counter = 0;
	current_player->can_go = true;
beginning:
	if (doubles_counter >= 3)
	{
		send_to_jail();
		current_player->can_go = false;
		return;
	}
	system("CLS");
	print_board();
start_again:
	std::string choice;
	int num_choices = print_menu(0);
	menu_it = menu.begin();
	std::cin >> choice;
	while (choice != "1" && choice != "2" && choice != "3" && choice != "4"&&choice != "5"&&choice != "6"&&choice!="7"&&choice!="8")
	{
		print_menu(0);
		std::cin >> choice;
	}
	while (std::stoi(choice) > num_choices||std::stoi(choice)<0)
	{
		print_menu(0);
		std::cin >> choice;
	}
	for (int i = 0; i < std::stoi(choice)-1; i++)
	{
		++menu_it;
	}
	if (menu_it == menu.end())
	{
		menu_it = menu.begin();
		goto start_again;
	}
	if (menu_it->second == 0)
	{
		if (current_player->current_location->can_be_owned && !current_player->current_location->is_owned)
		{
			start_auction();
		}
		int move;
		std::cin >> move;
		bool add_more = false;
		if (current_player->is_in_jail && !rolled_doubles)
		{
			current_player->jail_turn_counter++;
			goto beginning;
		}
		for (int i = 0; i < move; i++)
		{
			if (current_player->current_position->first == current_player->possible_positions.size() - 1 && i < move)
			{
				current_player->current_position = current_player->possible_positions.begin();
				current_player->has_made_first_pass = true;
				passed_go();
				i++;
				if (i<move)							//FIXXXXXXXXXXX
				{
					current_player->current_position++;
					//add_more = true;
				}
			}
			else
			{
				current_player->current_position++;
			}
		}
		if (add_more)			//making sure that going back around the board always works
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		if (current_player->is_in_jail)
		{
			rolled_doubles = false;
			current_player->is_in_jail = false;
			current_player->jail_turn_counter = 0;
		}
		if (current_player->current_position->first == go_to_jail_space)
		{
			send_to_jail();
			return;
		}
		else if (current_player->current_location->is_chance_square)
		{
			draw_chance();
		}
		else if (current_player->current_location->is_c_chest_square)
		{
			draw_community();
		}
		else if (current_player->current_location->is_owned&&current_player->current_location->owner != current_player)
		{
			std::cout << current_player->name << " landed on " << current_player->current_location->name << " which is owned by " << current_player->current_location->owner->name << std::endl;
			std::cout << "Taking $" << current_player->current_location->current_rent << " from " << current_player->name << " and giving it to " << current_player->current_location->owner->name << std::endl;
			current_player->money->amount -= current_player->current_location->current_rent;
			current_player->current_location->owner->money->amount += current_player->current_location->current_rent;
			system("PAUSE");
		}
		if (!rolled_doubles)
		{
			current_player->can_go = false;
		}
		goto beginning;

	}
	if (menu_it->second==1)
	{
		if (current_player->current_location->can_be_owned && !current_player->current_location->is_owned)
		{
			start_auction();
		}
		int move = roll_dice();
		bool add_more = false;
		if (current_player->is_in_jail && !rolled_doubles)
		{
			current_player->jail_turn_counter++;
			goto beginning;
		}
		for (int i = 0; i < move; i++)
		{
			if (current_player->current_position->first == current_player->possible_positions.size() - 1 && i < move)
			{
				current_player->current_position = current_player->possible_positions.begin();
				current_player->has_made_first_pass = true;
				passed_go();
				i++;
				if (i < move)							//FIXXXXXXXXXXX
				{
					current_player->current_position++;
					//add_more = true;
				}
			}
			else
			{
				current_player->current_position++;
			}
		}
		if (add_more)			//making sure that going back around the board always works
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		if (current_player->is_in_jail)
		{
			rolled_doubles = false;
			current_player->is_in_jail = false;
			current_player->jail_turn_counter = 0;
		}
		if (current_player->current_position->first == go_to_jail_space)
		{
			send_to_jail();
			return;
		}
		if (current_player->current_location->is_chance_square)
		{
			draw_chance();
		}
		if (current_player->current_location->is_c_chest_square)
		{
			draw_community();
		}
		if (current_player->current_location->is_owned&&current_player->current_location->owner != current_player)
		{
			std::cout << current_player->name << " landed on " << current_player->current_location->name << " which is owned by " << current_player->current_location->owner->name << std::endl;
			std::cout << "Taking $" << current_player->current_location->current_rent << " from " << current_player->name << " and giving it to " << current_player->current_location->owner->name << std::endl;
			current_player->money->amount -= current_player->current_location->current_rent;
			current_player->current_location->owner->money->amount += current_player->current_location->current_rent;
			system("PAUSE");
		}
		if (!rolled_doubles)
		{
			current_player->can_go = false;
		}
		goto beginning;

	}
	else if (menu_it->second==2)
	{
		current_player->print_properties();
		goto beginning;
	}
	else if (menu_it->second==3)
	{
		trade();
		goto beginning;
	}
	else if (menu_it->second == 4)
	{
		current_player->money->amount -= 50;
		current_player->is_in_jail = false;
		current_player->jail_turn_counter = 0;
		next_player();
	}
	else if (menu_it->second == 5)
	{
		current_player->num_of_GOJ--;
		current_player->is_in_jail = false;
		current_player->jail_turn_counter = 0;
		next_player();
	}
	else if (menu_it->second == 9)
	{
		if (current_player->current_location->can_be_owned && !current_player->current_location->is_owned)
		{
			start_auction();
		}
		next_player();
	}
	else if (menu_it->second == 7)
	{
		buy_property();
		goto beginning;
	}
	else if (menu_it->second == 6)
	{
		print_properties_left();
		goto beginning;
	}
	else if (menu_it->second == 10)
	{
		current_player->buy_houses();
	}
}

void board::ai_turn()
{
	system("CLS");
	std::map<double, Square*>::iterator it = current_player->values.begin();
	/*std::cout << "property values to player " << current_player->name << std::endl;
	//for (int i = 0; i < squares.size(); i++)
	//{
	//	std::cout << squares[i]->name << '\t' << find_property_value(squares[i], current_player) << std::endl;
	//}
	std::map<double, Square*>::iterator it = current_player->values.begin();
	check_values_for_trade();
	for (it = current_player->values.begin(); it != current_player->values.end(); ++it)
	{
		std::cout << it->second->name << '\t' << it->first << std::endl;
	}
	system("PAUSE");
	*/
	system("CLS");
	print_board();
	int move = roll_dice();
	bool add_more = false;
	if (current_player->is_in_jail && !rolled_doubles)
	{
		current_player->jail_turn_counter++;
	}
	for (int i = 0; i < move; i++)
	{
		if (current_player->current_position->first == current_player->possible_positions.size() - 1 && i < move)
		{
			current_player->current_position = current_player->possible_positions.begin();
			current_player->has_made_first_pass = true;
			passed_go();
			i++;
			if (i < move)							//FIXXXXXXXXXXX
			{
				current_player->current_position++;
				//add_more = true;
			}
		}
		else
		{
			current_player->current_position++;
		}
	}
	if (add_more)			//making sure that going back around the board always works
	{
		current_player->current_position++;
	}
	current_player->current_location = squares[current_player->current_position->first];
	if (current_player->is_in_jail)
	{
		rolled_doubles = false;
		current_player->is_in_jail = false;
		current_player->jail_turn_counter = 0;
	}
	if (current_player->current_position->first == go_to_jail_space)
	{
		send_to_jail();
		return;
	}
	if (current_player->current_location->is_chance_square)
	{
		draw_chance();
	}
	if (current_player->current_location->is_c_chest_square)
	{
		draw_community();
	}
	if (current_player->current_location->is_owned&&current_player->current_location->owner != current_player)
	{
		std::cout << current_player->name << " landed on " << current_player->current_location->name << " which is owned by " << current_player->current_location->owner->name << std::endl;
		std::cout << "Taking $" << current_player->current_location->current_rent << " from " << current_player->name << " and giving it to " << current_player->current_location->owner->name << std::endl;
		current_player->money->amount -= current_player->current_location->current_rent;
		current_player->current_location->owner->money->amount += current_player->current_location->current_rent;
		system("PAUSE");
	}
	if (!rolled_doubles)
	{
		current_player->can_go = false;
	}
	if (current_player->current_location->can_be_owned && !current_player->current_location->is_owned&&current_player->current_location->price < current_player->money->amount)
	{
		current_player->properties_owned.push_back(current_player->current_location);
		current_player->current_location->is_owned = true;
		current_player->current_location->owner = current_player;
		Board[current_player->current_location->house_num].second.first = current_player->current_location->num;
		Board[current_player->current_location->symbol_num].second.first = current_player->current_location->num;
		Board[current_player->current_location->letter_num].second.first = current_player->current_location->num;
		current_player->money->amount -= current_player->current_location->price;
	}
	else if (current_player->current_location->can_be_owned&&!current_player->current_location->is_owned)
	{
		start_auction();
	}
	check_values_for_trade();
	it = current_player->values.end();
	--it;																			//HERE 2/21
	int num_of_props_to_check = 7;
	int total = 0;
	for (int i = 0; i < players.size(); i++)
	{
		total += players[i]->properties_owned.size();
	}
	if (total >= 28)
	{
		num_of_props_to_check = 7;
		check_values_for_trade();
		it = current_player->values.end();
		--it;
	}
	for (int i = 0; i < num_of_props_to_check; i++)
	{
		if ((it->second->is_owned)&&(current_player->money->amount >= static_cast<int>(it->first))&&(it->second->owner!=current_player))
		{
			start_ai_trade(it->second, it->second->owner);
			break;
		}
		--it;
	}
}

void board::start_ai_trade(Square* wanted, Player* owning_player)
{
	std::map<double, Square*>::iterator it;
	for (it = current_player->values.begin(); it != current_player->values.end(); ++it)
	{
		if (it->second == wanted)
		{
			break;
		}
	}
	if (it == current_player->values.end())
	{
		std::cout << "Could not find property for some reason\n";
		system("PAUSE");
	}
	std::cout << current_player->name << " wants to trade " << wanted->name << " with " << owning_player->name <<" for $"<<static_cast<int>(it->first)<< std::endl;
	system("PAUSE");
	if (owning_player->is_player)
	{
		ai_to_player_trade(wanted, owning_player);
	}
	else
	{
		double owning_players_value = find_property_value(wanted, owning_player);
		if (owning_players_value < it->first)
		{
			std::cout << owning_player->name << " is willing to trade " << wanted->name <<std::endl;
			system("PAUSE");
			trading_player = owning_player;
			this->wanted.push_back(wanted);
			to_be_traded.push_back(new cash(static_cast<int>(it->first)));
			perform_trade();
		}
		else
		{
			std::cout << owning_player->name << " doesn't want to trade " << wanted->name << std::endl;
			system("PAUSE");
		}
	}
}


void board::ai_to_player_trade(Square* wanted, Player* owning_player)					
{
	std::string input;
	std::vector<Square*>::iterator iter;
	std::map<double, Square*>::iterator it;
	for (it = current_player->values.begin(); it != current_player->values.end(); ++it)
	{
		if (it->second == wanted)
		{
			break;
		}
	}
	if (it == current_player->values.end())
	{
		std::cout << "Could not find property for some reason\n";
		system("PAUSE");
	}
back:
	std::cout << current_player->name << " would like to trade " << wanted->name << " from " << owning_player->name << " for $" << it->first << std::endl;
	std::cout << "Do you agree?(Y/N)\n";
	std::cin >> input;
	if (input == "Y" || input == "y")
	{
		for (iter = owning_player->properties_owned.begin(); iter != owning_player->properties_owned.end(); ++iter)
		{
			if ((*iter)->name == wanted->name)
			{
				current_player->properties_owned.push_back(*iter);
				owning_player->properties_owned.erase(iter);
				owning_player->money->amount += static_cast<int>(current_player->most_valuable_property.first);
				current_player->money->amount -= static_cast<int>(current_player->most_valuable_property.first);
				break;
			}
		}
	}
	else if (input == "N" || input == "n")
	{
		return;
	}
	else
	{
		goto back;
	}
}

void board::check_values_for_trade()
{
	current_player->values.clear();
	std::map<double, Square*> temp_map;
	double total = 0;
	for (size_t i = 0; i < squares.size(); i++)
	{
		temp_map.insert(std::pair<double, Square*>(find_property_value(squares[i], current_player), squares[i]));
	}
	std::map<double, Square*>::iterator it;
	double largest = 0;
	for (it = temp_map.begin(); it != temp_map.end(); ++it)
	{
		total += it->first;
		if (it->first > largest)
		{
			largest = it->first;
			current_player->most_valuable_property = *it;
		}
	}
	total /= 28.0;
	for (size_t i = 0; i < squares.size(); i++)
	{
		current_player->values.insert(std::pair<double, Square*>(find_property_value(squares[i], current_player), squares[i]));
	}
							///					HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE 2/19
}

double board::num_of_turns_left()
{
	std::vector<double> player_wealth;
	double total_wealth = 0;
	for (int i = 0; i < players.size(); i++)
	{
		player_wealth.push_back(players[i]->get_wealth());
		total_wealth += player_wealth[i];
	}
	double largest_percentage = 0;
	for (int i = 0; i < player_wealth.size(); i++)
	{
		double current_percentage = player_wealth[i] / total_wealth;
		if (current_percentage > largest_percentage)
		{
			largest_percentage = current_percentage;
		}
	}
	return ((1-largest_percentage) * 50.0);
}

double board::find_property_value(Square* current, Player* POI)		//use file to figure out thing; should be num of other players* num in file * (avg num of turns - curent turns)
{
	double value = 0;
	if (current->probabilities.size() == 0)
	{
		return 0;
	}
	switch (current->color_num)
	{
	case PURPLE:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (2.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->purp_owned));
		break;
	}
	case LIGHT_BLUE:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->lb_owned));		break;
	}
	case PINK:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->pink_owned));		break;
	}
	case ORANGE:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->oj_owned));		break;
	}
	case RED:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->red_owned));		break;
	}
	case YELLOW:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->yellow_owned));		break;
	}
	case GREEN:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (3.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->green_owned));		break;
	}
	case BLUE:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left())*(static_cast<double>(POI->money->amount) / (2.0*5.0*static_cast<double>(current->price_per_house)))*(pow(1.8, POI->blue_owned));		break;
	}
	case UTILITY:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left());		break;
	}
	case RAILROAD:
	{
		value = current->probabilities[current->probabilities.size() - 1] *(num_of_turns_left());		break;
	}
	}
	//value *= .84;
	//value *= current->price;
	return value;
}

void board::average_property_probabilities()
{
	for (int i = 0; i < squares.size(); i++)
	{
		if (squares[i]->probabilities.size() == 0)
		{
			continue;
		}
		std::sort(squares[i]->probabilities.begin(), squares[i]->probabilities.end());
		std::cout << squares[i]->probabilities[squares[i]->probabilities.size() - 1] << " " << squares[i]->probabilities[squares[i]->probabilities.size() - 2] << std::endl;
		std::cout << squares[i]->probabilities[squares[i]->probabilities.size() - 1] + squares[i]->probabilities[squares[i]->probabilities.size() - 2] << std::endl;
		double value = (squares[i]->probabilities[squares[i]->probabilities.size() - 1] + squares[i]->probabilities[squares[i]->probabilities.size() - 2]) / 2.0;
		squares[i]->probabilities.push_back(value);
	}
	std::cout << "probabilities for " << squares[19]->name << std::endl;
	for (int i = 0; i < squares[19]->probabilities.size(); i++)
	{
		std::cout << squares[19]->probabilities[i] << std::endl;
	}
}

void board::start_auction()
{
	std::cout <<current_player->name<< " did not purchase " << current_player->current_location->name << "! And the property will now go up for Auction.\n";
	int num_of_bids = 1;
	int highest_bid = 0;
	std::string input_bid;
	while (num_of_bids >= 1)
	{
		std::cout << "Current bid is: $" << highest_bid << std::endl;
		num_of_bids = 0;
		for (int i = 0; i < players.size(); i++)
		{
			tryagain:
			std::cout << players[i]->name << " how much would you like to bid? (Bid $0 for a null bid)\n";
			std::cin >> input_bid;
			if (input_bid >= "0"&&input_bid < "9999")
			{
				if (std::stoi(input_bid) >= highest_bid&&std::stoi(input_bid)<players[i]->money->amount)
				{
					highest_bid = std::stoi(input_bid);
					highest_bidder = players[i];
					num_of_bids++;
					std::cout << "New highest bid of $" << highest_bid << "!\n";
				}
			}
			else
			{
				std::cout << "Please enter a valid numbern\n";
				goto tryagain;
			}
		}
	}
	std::cout << "Congrats! " << highest_bidder->name << " won the Auction with a bid of " << highest_bid << std::endl;
	current_player->current_location->is_owned = true;
	current_player->current_location->owner = highest_bidder;
	highest_bidder->properties_owned.push_back(current_player->current_location);
	Board[current_player->current_location->house_num].second.first = current_player->current_location->num;
	Board[current_player->current_location->symbol_num].second.first = current_player->current_location->num;
	Board[current_player->current_location->letter_num].second.first = current_player->current_location->num;
	highest_bidder->money->amount -= highest_bid;
	if (check_for_monopoly(current_player->current_location, highest_bidder))
	{
		std::cout << "Congratulations you got a monopoly!\n";
		system("PAUSE");
	}
}

void board::send_to_jail()
{
	std::cout << "Uh-Oh! " << current_player->name << " got sent to Jail!\n";
	current_player->current_position = current_player->possible_positions.begin();
	for (int i = 0; i < 10; i++)
		current_player->current_position++;
	current_player->current_location = squares[current_player->current_position->first];
	current_player->is_in_jail = true;
	next_player();
}

void board::buy_property()
{
	std::cout << "Are you sure you want to buy " << current_player->current_location->name << " for $" << current_player->current_location->price << "? (Y/N)\n";
	std::string input;
	std::cin >> input;
	while (input != "Y" && input != "N" && input != "y" && input != "n")
	{
		std::cout << "Are you sure you want to buy " << current_player->current_location->name << " for $" << current_player->current_location->price << "? (Y/N)\n";
		std::cin >> input;
	}
	if (input == "Y" || input == "y")
	{
		current_player->money->amount -= current_player->current_location->price;
		if (current_player->money->amount < 0)
		{
			std::cout << "You do not have enough money to buy this property!\n";
			current_player->money->amount += current_player->current_location->price;
			goto end;
		}
		else
		{
			current_player->properties_owned.push_back(current_player->current_location);
			current_player->current_location->is_owned = true;
			current_player->current_location->owner = current_player;
			Board[current_player->current_location->house_num].second.first = current_player->current_location->num;
			Board[current_player->current_location->symbol_num].second.first = current_player->current_location->num;
			Board[current_player->current_location->letter_num].second.first = current_player->current_location->num;
			if (check_for_monopoly(current_player->current_location, current_player))
			{
				std::cout << "Congratulations you got a monopoly!\n";
				system("PAUSE");
			}
		}
	}
	else
	{
		goto end;
	}
end:
	;
}

bool board::check_for_monopoly(Square* bought, Player* buying_player)
{
	switch (bought->color_num)
	{
	case PURPLE:
	{
		buying_player->purp_owned++;
		if (buying_player->purp_owned == 2)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == PURPLE)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case LIGHT_BLUE:
	{
		buying_player->lb_owned++;
		if (buying_player->lb_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == LIGHT_BLUE)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case PINK:
	{
		buying_player->pink_owned++;
		if (buying_player->pink_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == PINK)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case ORANGE:
	{
		buying_player->oj_owned++;
		if (buying_player->oj_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == ORANGE)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case RED:
	{
		buying_player->red_owned++;
		if (buying_player->red_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == RED)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case YELLOW:
	{
		buying_player->yellow_owned++;
		if (buying_player->yellow_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == YELLOW)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case GREEN:
	{
		buying_player->green_owned++;
		if (buying_player->green_owned == 3)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == GREEN)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case BLUE:
	{
		buying_player->blue_owned++;
		if (buying_player->blue_owned == 2)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == BLUE)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case UTILITY:
	{
		buying_player->util_owned++;
		if (buying_player->util_owned == 2)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == UTILITY)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	case RAILROAD:
	{
		buying_player->rr_owned++;
		if (buying_player->rr_owned == 4)
		{
			std::vector<Square*>* v1 = new std::vector<Square*>;
			for (int i = 0; i < buying_player->properties_owned.size(); i++)
			{
				if (buying_player->properties_owned[i]->color_num == RAILROAD)
				{
					v1->push_back(buying_player->properties_owned[i]);
					buying_player->properties_owned[i]->is_in_a_monopoly = true;
				}
			}
			buying_player->monopolies_vector.push_back(v1);
			buying_player->monopolies++;
			return true;
		}
		break;
	}
	}
	return false;
}

void board::trade()
{
	int selection = 1;
	std::string player_choice;
	std::cout << "Please select a player.\n";
	for (int i = 1; i < players.size()+1; i++)
	{													//just really want to start printing at 1 even if first player wants to trade.
		if (players[i-1] == current_player)
		{
			continue;
		}
		std::cout << selection << ". " << players[i-1]->name << std::endl;
		selection++;
	}
	std::cin >> player_choice;
	while (std::stoi(player_choice) > players.size() || std::stoi(player_choice) < 0)
	{
		std::cin >> player_choice;
	}
	if (current_player->number>std::stoi(player_choice))
	{
		trading_player = players[std::stoi(player_choice) - 1];
	}
	else
	{
		trading_player = players[std::stoi(player_choice)];
	}
again:
	std::cout << "What would you like from "<<trading_player->name<<"?\n";
	std::cout << "1. Properties\n2. Free Passes\n3. Get out of Jail card\n4. Money\n5. End Trade\n";
	std::cin >> player_choice;
	while (player_choice != "1"&&player_choice != "2"&&player_choice != "3"&&player_choice!="4"&&player_choice!="5")
	{
		std::cout << "1. Properties\n2. Free Passes\n3. Get out of Jail card\n4. Money\n5. End Trade";
		std::cin >> player_choice;
	}
	if (player_choice == "1")
	{
		int prop_choice = 0;
		int number_of_missing = trading_player->print_prop_names(wanted);
		if (trading_player->properties_owned.size() <= 0)
		{
			std::cout << "This player has no properties to trade!\nTry Again!\n";
			goto again;
		}
		else
		{
			while (std::cin.fail() || prop_choice<1 || prop_choice>trading_player->properties_owned.size())
			{
				std::cin >> prop_choice;
			}
			if (prop_choice > number_of_missing)
			{
				wanted.push_back(trading_player->properties_owned[prop_choice]);
				std::cout << trading_player->properties_owned[prop_choice]->name << " has been added to trading list.\n";
			}
			else
			{
				wanted.push_back(trading_player->properties_owned[prop_choice - 1]);
				std::cout << trading_player->properties_owned[prop_choice - 1]->name << " has been added to trading list.\n";
			}
			goto again;
		}
	}
	if (player_choice == "2")
	{
	back:
		int num;
		std::cout << "How many free passes do you want from " << trading_player->name << std::endl;
		std::cin >> num;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout << "Please enter a valid number\n";
			goto back;
		}
		wanted.push_back(new free_pass(trading_player, num));
		std::cout << num << " number of free passes has been added to trade.\n";
		goto again;
	}
	if (player_choice == "3")
	{
		if (trading_player->num_of_GOJ>0)
		{
			wanted.push_back(trading_player->get_out_card);
			std::cout << trading_player->name << "'s Get out of Jail Free card has been added to the trade.\n";
			goto again;
		}
		else
		{
			std::cout << trading_player->name << " doesn't have a Get out of Jail Free card!\n";
			goto again;
		}
	}
	if (player_choice == "4")
	{
		int amount = -1;
		std::cout << "Please enter desired amount ("<<trading_player->name<<" has $"<<trading_player->money->amount<<"):\n";
		while (std::cin.fail() || amount<0 || amount>trading_player->money->amount)
		{
			std::cin >> amount;
		}
		std::cout << "$" << amount << " from " << trading_player->name << " has been added to the trade.\n";
		wanted.push_back(new cash(amount));
		goto again;
	}
	if (player_choice == "5")
	{
		std::cout << "Are you sure you are done trading? (Y/N)\n";
		std::string input;
		std::cin >> input;
		while (input != "Y" && input != "N" && input != "y" && input != "n")
		{
			std::cout << "Are you sure you are done trading? (Y/N)\n";
			std::cin >> input;
		}
		if (input == "y" || input == "Y")
		{
			std::string input_pin;
			std::cout << "Please enter your trading PIN to confirm.\n";
			std::cin >> input_pin;
			int num_pin = std::stoi(input_pin);
			while (num_pin != current_player->PIN)
			{
				std::cout << "Wrong PIN, please try again.\n";
				std::cin >> input_pin;
				num_pin = std::stoi(input_pin);
			}
			goto give;
		}
		else
		{
			goto again;
		}
	}
give:
	std::cout << "What are you willing to give in exchange? \n";
	std::cout << "1. Properties\n2. Free Passes\n3. Get out of Jail card\n4. Money\n5. End Trade\n";
	std::cin >> player_choice;
	while (player_choice != "1"&&player_choice != "2"&&player_choice != "3"&&player_choice != "4"&&player_choice != "5")
	{
		std::cout << "1. Properties\n2. Free Passes\n3. Get out of Jail card\n4. Money\n5. End Trade";
		std::cin >> player_choice;
	}
	if (player_choice == "1")
	{
		std::cout << "Which property would you like to give?\n";
		int num_of_missing = current_player->print_prop_names(to_be_traded);
		int prop_choice = 0;
		if (current_player->properties_owned.size() <= 0)
		{
			std::cout << "You have no properties to trade!\nTry Again!\n";
			goto give;
		}
		else
		{
			while (std::cin.fail() || prop_choice<1 || prop_choice>current_player->properties_owned.size())
			{
				std::cin >> prop_choice;
			}
			if (prop_choice > num_of_missing)
			{
				to_be_traded.push_back(current_player->properties_owned[prop_choice]);
				std::cout << current_player->properties_owned[prop_choice]->name << " has been added to trading list.\n";
			}
			else
			{
				to_be_traded.push_back(current_player->properties_owned[prop_choice - 1]);
				std::cout << current_player->properties_owned[prop_choice - 1]->name << " has been added to trading list.\n";
			}
			goto give;
		}
	}
	else if (player_choice == "2")
	{
	back2:
		int num;
		std::cout << "How many free passes are you willing to give?" << std::endl;
		std::cin >> num;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cout << "Please enter a valid number\n";
			goto back2;
		}
		to_be_traded.push_back(new free_pass(current_player, num));
		std::cout << num << " number of free passes has been added to trade.\n";
		goto give;
	}
	else if (player_choice == "3")
	{
		if (current_player->num_of_GOJ>0)
		{
			to_be_traded.push_back(current_player->get_out_card);
			std::cout << "Get out of jail card has been added to trade.\n";
		}
		else
		{
			std::cout << "You do not have a get out of jail free card!\n";
		}
		goto give;
	}
	else if (player_choice == "4")
	{
		int amount = -1;
		std::cout << "Please enter desired amount (You have $" << current_player->money->amount << "):\n";
		while (std::cin.fail() || amount<0 || amount>trading_player->money->amount)
		{
			std::cin >> amount;
		}
		std::cout << "$" << amount << " from " << trading_player->name << " has been added to the trade.\n";
		wanted.push_back(new cash(amount));
		goto give;
	}
	else if (player_choice == "5")
	{
		std::cout << "Are you sure you are done trading? (Y/N)\n";
		std::string input;
		std::cin >> input;
		while (input != "Y" && input != "N" && input != "y" && input != "n")
		{
			std::cout << "Are you sure you are done trading? (Y/N)\n";
			std::cin >> input;
		}
		if (input == "y" || input == "Y")
		{
			std::string input_pin;
			std::cout << "Please enter your trading PIN to confirm.\n";
			std::cin >> input_pin;
			int num_pin = std::stoi(input_pin);
			while (num_pin != trading_player->PIN)
			{
				std::cout << "Wrong PIN, please try again.\n";
				std::cin >> input_pin;
				num_pin = std::stoi(input_pin);
			}
			goto exit;
		}
		else
		{
			goto give;
		}
	}
exit:
	print_trades();
	perform_trade();
}

int board::roll_dice()
{
	int num1, num2; 
	std::srand(std::time(NULL));
	for (int i = 2500; i >0 ; i--)							
	{
		num1 = std::rand() % 6 + 1;
		num2 = std::rand() % 6 + 1;
		std::cout << "\r";
		if (i <= 50 && i > 25&&current_player->is_player)
		{
			Sleep(20);
		}
		if (i <= 25 && i > 13 && current_player->is_player)
		{
			Sleep(75);
		}
		if (i <= 13 && i > 3 && current_player->is_player)
		{
			Sleep(100);
		}
		if (i <= 3 && current_player->is_player)
		{
			Sleep(500);
		}
		if (i == 1 && current_player->is_player)
		{
			Sleep(750);
		}
		std::cout << num1 << " " << num2;
	}
	std::cout << std::endl;
	std::cout << num1 + num2 << "!\n";
	if (rolled_doubles)
	{
		rolled_doubles = false;
	}
	if (num1 == num2)
	{
		rolled_doubles = true;
		doubles_counter++;
		std::cout << "Doubles!\n";
	}
	if (current_player->is_player)
	{
		system("PAUSE");
	}
	else
	{
		Sleep(1000);
	}
	return (num1 + num2);
}

void board::print_info(int num)
{
	std::vector<std::string> info;
	std::string s = "";
	s = current_player->name + "'s turn.";
	info.push_back(s);
	s = "Number of properties owned by " + current_player->name + ": " + std::to_string(current_player->properties_owned.size());
	info.push_back(s);
	for (int i = 0; i < current_player->properties_owned.size(); i++)
	{
		info.push_back("\t" + current_player->properties_owned[i]->name);
	}
	info.push_back("Current position: " + current_player->current_location->name);
	info.push_back("Monopolies owned/Houses owned/Hotels owned: " + std::to_string(current_player->monopolies) + "/" + std::to_string(current_player->houses) + "/" + std::to_string(current_player->hotels));
	info.push_back("Current Money: " + std::to_string(current_player->money->amount));
	info.push_back(" ");
	for (int i = 0; i < players.size(); i++)
	{
		info.push_back(players[i]->name + " ");
	}
	std::cout << info[num];
	if (num > 5 + current_player->properties_owned.size())
	{
		players[num - (6 + current_player->properties_owned.size())]->token->print();
	}
}

void board::print_trades()
{
	std::cout << std::endl << "Items to be traded from " << trading_player->name<<":\n";
	for (int i = 0; i < wanted.size(); i++)
	{
		std::cout << i << ". ";
		wanted[i]->print_trade();
		std::cout << std::endl;
	}
	std::cout << std::endl << "Items to be traded from " << current_player->name << ":\n";
	for (int i = 0; i < to_be_traded.size(); i++)
	{
		std::cout << i << ". ";
		to_be_traded[i]->print_trade();
		std::cout << std::endl;
	}
	system("PAUSE");
}

void board::passed_go()
{
	current_player->money->amount += 200;
}

void board::print_cards()
{
	for (int i = 0; i < chance_cards.size(); i++)
	{
		chance_cards[i]->print();
		std::cout << "\n";
	}
	for (int i = 0; i < community_chest_cards.size(); i++)
	{
		community_chest_cards[i]->print();
		std::cout << "\n";
	}
}

void board::draw_chance()
{
	std::cout << "You pulled a chance card!\n";
	chance_cards[0]->print();
	do_chance(chance_cards[0]->get_num());
	std::rotate(chance_cards.begin(), chance_cards.begin()+1, chance_cards.end());
	shuffle_counter_chance++;
	if (shuffle_counter_chance == chance_cards.size())
	{
		std::random_shuffle(chance_cards.begin(), chance_cards.end());
		shuffle_counter_chance = 0;
	}
	system("PAUSE");
}

void board::draw_community()
{
	std::cout << "You pulled a community chest card!\n";
	community_chest_cards[0]->print();
	do_cc(community_chest_cards[0]->get_num());
	std::rotate(community_chest_cards.begin(), community_chest_cards.begin() + 1, community_chest_cards.end());
	shuffle_counter_cc++;
	if (shuffle_counter_cc == community_chest_cards.size())
	{
		std::random_shuffle(community_chest_cards.begin(), community_chest_cards.end());
		shuffle_counter_cc = 0;
	}
	system("PAUSE");
}

void board::do_chance(int num)
{
	switch (num)
	{
	case 0:
	{
		current_player->current_position = current_player->possible_positions.begin();
		current_player->current_location = squares[current_player->current_position->first];
		passed_go();
		break;
	}
	case 1:
	{
		if (current_player->current_position->first > 24)
		{
			passed_go();
		}
		current_player->current_position = current_player->possible_positions.begin();
		for (int i = 0; i < 24; i++)
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 2:
	{
		if (current_player->current_position->first > 11)
		{
			passed_go();
		}
		current_player->current_position = current_player->possible_positions.begin();
		for (int i = 0; i < 11; i++)
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 3:
	{
		if (current_player->current_position->first < 12)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 12; i++)
			{
				current_player->current_position++;
			}
		}
		if (current_player->current_position->first < 28)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 28; i++)
			{
				current_player->current_position++;
			}
		}
		else
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 12; i++)
			{
				current_player->current_position++;
			}
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 4:
	{
		if (current_player->current_position->first < 5)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 5; i++)
			{
				current_player->current_position++;
			}
		}
		else if (current_player->current_position->first < 15)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 15; i++)
			{
				current_player->current_position++;
			}
		}
		else if (current_player->current_position->first < 25)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 25; i++)
			{
				current_player->current_position++;
			}
		}
		else if (current_player->current_position->first < 35)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 35; i++)
			{
				current_player->current_position++;
			}
		}
		else if (current_player->current_position->first > 35)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < 5; i++)
			{
				current_player->current_position++;
			}
			passed_go();
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 5:
	{
		current_player->money->amount += 50;
		break;
	}
	case 6:
	{
		current_player->num_of_GOJ++;
		current_player->get_out_card = chance_cards[num];
		break;
	}
	case 7:
	{
		int current_temp_position = current_player->current_position->first;
		if (current_player->current_position->first > 3)
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < current_temp_position - 3; i++)
			{
				current_player->current_position++;
			}
		}
		else
		{
			current_player->current_position = current_player->possible_positions.begin();
			for (int i = 0; i < current_temp_position - 3; i++)
			{
				current_player->current_position++;
			}
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 8:
	{
		send_to_jail();
		break;
	}
	case 9:
	{
		current_player->money->amount -= (25 * current_player->houses);
		current_player->money->amount -= (100 * current_player->hotels);
		std::cout << "Deducting " << current_player->houses << " * $25 + " << current_player->hotels << " * $100 = $" << current_player->houses * 25 + current_player->hotels * 100 << std::endl;
		break;
	}
	case 10:
	{
		current_player->money->amount -= 15;
		break;
	}
	case 11:
	{
		if (current_player->current_position->first > 5)
		{
			passed_go();
		}
		current_player->current_position = current_player->possible_positions.begin();
		for (int i = 0; i < 5; i++)
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 12:
	{
		current_player->current_position = current_player->possible_positions.begin();
		for (int i = 0; i < 39; i++)
		{
			current_player->current_position++;
		}
		current_player->current_location = squares[current_player->current_position->first];
		break;
	}
	case 13:
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i] == current_player)
				continue;
			else
			{
				current_player->money->amount -= 50;
				players[i]->money->amount += 50;
				std::cout << "Taking $50 from " << current_player->name;
				std::cout << " and giving it to " << players[i]->name << std::endl;
			}
		}
		break;
	}
	case 14:
	{
		current_player->money->amount += 150;
		break;
	}
	case 15:
	{
		current_player->money->amount += 100;
		break;
	}
	}
}

void board::do_cc(int num)
{
	switch (num)
	{
	case 0:
	{
		current_player->current_position = current_player->possible_positions.begin();
		current_player->current_location = squares[current_player->current_position->first];
		passed_go();
		break;
	}
	case 1:
	{
		current_player->money->amount += 200;
		break;
	}
	case 2:
	{
		current_player->money->amount -= 50;
		break;
	}
	case 3:
	{
		current_player->money->amount += 50;
		break;
	}
	case 4:
	{
		current_player->num_of_GOJ++;
		current_player->get_out_card = community_chest_cards[num];
		break;
	}
	case 5:
	{
		send_to_jail();
		break;
	}
	case 6:
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i] == current_player)
				continue;
			else
			{
				current_player->money->amount += 50;
				players[i]->money->amount -= 50;
				std::cout << "Taking $50 from " << players[i]->name;
				std::cout << " and giving it to " << current_player->name << std::endl;
			}
		}
		break;
	}
	case 7:
	{
		current_player->money->amount += 100;
		break;
	}
	case 8:
	{
		current_player->money->amount += 20;
		break;
	}
	case 9:
	{
		current_player->money->amount += 10;
		break;
	}
	case 10:
	{
		current_player->money->amount += 100;
		break;
	}
	case 11:
	{
		current_player->money->amount -= 100;
		break;
	}
	case 12:
	{
		current_player->money->amount -= 150;
		break;
	}
	case 13:
	{
		current_player->money->amount += 25;
		break;
	}
	case 14:
	{
		current_player->money->amount -= (40 * current_player->houses);
		current_player->money->amount -= (115 * current_player->hotels);
		std::cout << "Deducting " << current_player->houses << " * $40 + " << current_player->hotels << " * $115 = $" << current_player->houses * 40 + current_player->hotels * 115 << std::endl;
		break;
	}
	case 15:
	{
		current_player->money->amount += 10;
		break;
	}
	case 16:
	{
		current_player->money->amount += 100;
		break;
	}
	}
}

void board::print_properties_left()
{
	for (int i = 0; i < squares.size(); i++)
	{
		if (!squares[i]->is_owned&&squares[i]->can_be_owned)
		{
			squares[i]->print_name();
		}
	}
	system("PAUSE");
}

void board::perform_trade()
{

	for (int i = 0; i < to_be_traded.size(); i++)
	{
		if (to_be_traded[i]->type == "square")
		{
			std::vector<Square*>::iterator iter;
			for (iter = trading_player->properties_owned.begin(); iter != trading_player->properties_owned.end(); iter++)
			{
				if ((*iter)->name == to_be_traded[i]->get_name());
				{
					current_player->properties_owned.push_back(*iter);
					trading_player->properties_owned.erase(iter);
					break;
				}
			}
		}
		if (to_be_traded[i]->type == "chance" || to_be_traded[i]->type == "community_chest")
		{
			trading_player->num_of_GOJ--;
			current_player->num_of_GOJ++;
		}
		if (to_be_traded[i]->type == "cash")
		{
			trading_player->money->amount -= to_be_traded[i]->get_cash_amount();
			current_player->money->amount +\\\\\\\\\\\\\\\\\\\\\= to_be_traded[i]->get_cash_amount();
		}
		to_be_traded[i]->owner = trading_player;
	}
	for (int i = 0; i < wanted.size(); i++)
	{
		if (wanted[i]->type == "square")
		{
			std::vector<Square*>::iterator iter2;
			for (iter2 = current_player->properties_owned.begin(); iter2 != current_player->properties_owned.end(); iter2++)
			{
				if ((*iter2)->name == wanted[i]->get_name());
				{
					trading_player->properties_owned.push_back(*iter2);
					current_player->properties_owned.erase(iter2);
					break;
				}
			}
		}
		if (wanted[i]->type == "chance" || wanted[i]->type == "community_chest")
		{
			trading_player->num_of_GOJ++;
			current_player->num_of_GOJ--;
		}
		if (wanted[i]->type == "cash")
		{
			trading_player->money->amount += wanted[i]->get_cash_amount();
			current_player->money->amount -= wanted[i]->get_cash_amount();
		}
		wanted[i]->owner = current_player;
	}
	for (int i = 0; i < wanted.size(); i++)
	{
		wanted.pop_back();
	}
	for (int i = 0; i < to_be_traded.size(); i++)
	{
		to_be_traded.pop_back();
	}
}

board::~board()
{
	for (int i = 0; i < players.size(); i++)
	{
		for (int j = 0; j < players[i]->monopolies_vector.size(); j++)
		{
			delete players[i]->monopolies_vector[j];
		}
	}
	for (int i = 0; i < squares.size(); i++)
	{
		delete squares[i];
	}
	for (int i = 0; i < players.size(); i++)
	{
		delete players[i];
	}
	for (int i = 0; i < chance_cards.size(); i++)
	{
		delete chance_cards[i];
	}
	for (int i = 0; i < community_chest_cards.size(); i++)
	{
		delete community_chest_cards[i];
	}
}



/*
purple		85
cc
purple
INC TAX
RR				0
light blue		176
chance			240
light blue
light blue
jail			192
pink			208
utility
pink
pink
RR
orange			96
cc
orange
orange
free
red				64
chance
red
red
RR
yellow			224
yellow
utility
yellow
go to jail
green			32/160
green
cc
green
RR
chance
dark blue			144
LUX TAX				0
dark blue
GO











should probably start working on AI

need to make GOJ cards not rotate back into deck but do go back in once you use them.

mortgaging properties

maybe look into a fast game method (play for like 10-15 minutes especially just to beta)

need to implement win conditions

buying and selling of houses

*/


/*
make int in second pair equal to index number in squares list (1-39) so >0 will still work
make bool in print function so that when text needs to be shown bool is true
before printing check is bool is true
if it is then 

*/

/*
for monopolies maybe check color nums to see if they are part of same monopoly but dark blue and purple will be tricky cause 2
could also check size of each vector inside of monopoly vectors and see if size is equal to two or three
have integer for each player dictating how many properties of each color they have,
each time a property is bought, number will go up, then check that number against number of properties required to have monopoly
then if it is a monopoly push back those into a monopoly vector inside monopolies vector using color num and cycling through properties owned
utilities should be done the same way to dictate 10 v 4 times dice throw.
*/




/*
2/10
auctions could use some work
still need trade to work better
once all are bought need to increase trading

*/