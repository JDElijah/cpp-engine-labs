/*
Lab 04: Input Commands

This exercise separate raw input from gameplay behavior by converting keyboard characters into named commands. 

Core C++ concepts practiced: 
	- enum class: defines a limited set of valid commands
	- switch statements: maps input to commands and executes behavior 
	- functions: separates input mapping, command execution, and rendering
	- references: allows ExecuteCommand to modify the real Player and running flag
	- const references: allows RenderPlayer to read player data without modifying it

The command flow is: 
	
	raw input -> Command enum -> ExecuteCommand -> update game state

This is better than spreading raw keys like 'a', 'd', 'f', or 'q' throughout the gameplay code.
The game should care about actions such as MoveLeft, MoveRight, Attack, Heal, or Quit, not the physical key that 
triggered them. 

This models an important game development idea: 
input handling should be decoubled from gameplay behavior. 

This idea can expand into: 
	- input remapping 
	- controller support
	- AI using the same commands as the player 
	- replay systems
	- undo/redo systems
	- networked gameplay commands 
	- cleaner game loop architecture

The main idea: 
Translate low-level input into meaningful game commands, then execute those commands to update game state. 


*/
#include <iostream>
#include <string>

enum class Command
{
	MoveLeft,
	MoveRight,
	Attack,
	Heal,
	Quit,
	Unknown
};

struct Player
{
	int x;
	int health;
	int stamina;
};

Command GetCommandFromInput(char input)
{
	switch (input)
	{
		case 'a':
			return Command::MoveLeft;
		
		case 'd':
			return Command::MoveRight;

		case 'f':
			return Command::Attack;

		case 'h':
			return Command::Heal; 

		case 'q': 
			return Command::Quit; 

		default:
			return Command::Unknown; 

	}
}

std::string CommandToString(Command command)
{
	switch (command)
	{
		case Command::MoveLeft: 
			return "MoveLeft"; 

		case Command::MoveRight:
			return "MoveRight"; 

		case Command::Attack:
			return "Attack"; 

		case Command::Heal:
			return "Heal"; 

		case Command::Quit:
			return "Quit"; 

		case Command::Unknown:
			return "Unknown"; 

		default:
			return "Invalid"; 
	}
}

void ExecuteCommand(Command command, Player& player, bool& running)
{
	switch (command)
	{
		case Command::MoveLeft:
			player.x--; 
			break; 
		
		case Command::MoveRight:
			player.x++; 
			break; 

		case Command::Attack:
			if (player.stamina >= 10)
			{
				std::cout << "Player attacks!\n";
				player.stamina -= 10;
			}
			else
			{
				std::cout << "Not enough stamina to attack!\n";
			}
			break;

		case Command::Heal:
			if (player.health < 100)
			{
				player.health += 10; 
				if (player.health > 100)
				{
					player.health = 100; 
				}
			}
			break;

		case Command::Quit:
			running = false;
			break; 

		case Command::Unknown:
			std::cout << "Unknown command.\n"; 
			break; 
	}
}

void RenderPlayer(const Player& player)
{
	std::cout << "Player X Position: " << player.x << "\n";
	std::cout << "Player Health: " << player.health << "\n"; 
	std::cout << "Player Stamina: " << player.stamina << "\n";
	std::cout << "----------------------\n";
}

int main()
{
	bool running = true; 
	int frame = 0;

	Player player
	{
		0,
		55, 
		30
	};
	RenderPlayer(player); 

	while (running)
	{
		char input; 

		std::cout << "Command? move left/right, attack, heal, quit (a/d/f/h/q): "; 
		std::cin >> input; 

		Command command = GetCommandFromInput(input);

		std::cout << "Command received: " << CommandToString(command) << "\n"; 

		ExecuteCommand(command, player, running); 

		frame++; 

		std::cout << "Frame: " << frame << "\n";
		RenderPlayer(player);
	}
	
	std::cout << "Game ended.\n";

	return 0; 

}

