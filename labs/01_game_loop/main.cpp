/*

Lab 01: Game Loop 

This exerise demonstrates the basic structure of a game loop: 

1. Process input 
2. Update game state
3. Render/output the current game state
4. Repeat while the game is running 

The program tracks simple game state: 
- running: controls whether the loop continues 
- frame: counts how many update cycles have occured 
- playerX: represents the player's position 
- playerHealth: represents the player's health

Player input changes the state: 
- 'a' moves the player left 
- 'd' moves the player right 
- 'h' damages the player
- 'q' quits the loop 

This models an important game development idea: 
	Input shopuld change game state, and output should display that state

The health check introduces a simple game rule: 
	If playerHealth reaches 0 or below, the game ends. 

This small console program mirrors the core idea behind real-time games and engines: 
	State changes over time inside a repeated update loop. 

*/


#include <iostream>

int main()
{
	bool running = true; 
	int frame = 0; 
	int playerX = 0; 
	int playerHealth = 100;

	while (running)
	{
		char input; 

		std::cout << "Move left/right or quit? (a/d/h/q): "; 
		std::cin >> input; 

		if (input == 'q')
		{
			running = false;
		}
		else if (input == 'a')
		{
			playerX--; 
		}
		else if (input == 'd')
		{
			playerX++; 
		}
		else if (input == 'h')
		{
			playerHealth = playerHealth - 10;
			if (playerHealth <= 0)
			{
				std::cout << "Player died. Game over." << "\n";
				running = false; 
			}
		}

		frame++; 

		std::cout << "Frame: " << frame << "\n";
		std::cout << "Player X Position: " << playerX << "\n";
		std::cout << "Player health: " << playerHealth << "\n"; 
		std::cout << "----------------------\n";
	}

	return 0; 
}
