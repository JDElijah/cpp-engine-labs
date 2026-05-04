/*
Lab 05: Functions and Separation

This exercise refactors a simple game loop by separating the program into clear functions: 

	ProcessInput -> handles player input 
	UpdateGame -> applies game rules and advances simulation state 
	RenderGame -> displays the current state

Core C++ concepts practiced: 
	- struct: groups game state into one object 
	- functions: separates responsibilities into smaller units 
	- reference: allows ProcessInput and UpdateGame to modify the real GameState
	- const references: allows RenderGame to read state without modifying it 

The GameState struct stores the simulation state: 
	- running 
	- frame
	- playerX
	- playerHealth
	- score

This models an important game development pattern: 
	input phase -> update phase -> render/output phase

Separating these phases keeps code easier to read, debug, and expand. A real engine may hide this structure, but the same idea appears in game loops,
Unreal Tick Logic, simulation systems, UI updtes, and rendering pipelines. 

This lab also introduces separation of concerns: 
each  function should have a clear job instead of putting all logic directly in main

The main idea: 
organize the game loop into focused phases so state changes, game rules, and output are easier to understand and maintain. 

*/

#include <iostream>

struct GameState
{
	bool running; 
	int frame; 
	int playerX; 
	int playerHealth;
	int score;
};

void ProcessInput(GameState& game) {
	
	char input;

	std::cout << "Move left/right, take damage, collect coin, or quit? (a/d/h/c/q): "; 
	std::cin >> input; 

	if (input == 'q')
	{
		game.running = false; 
	}
	else if (input == 'a')
	{
		game.playerX--; 
	}
	else if (input == 'd')
	{
		game.playerX++; 
	}
	else if (input == 'h')
	{
		game.playerHealth -= 10; 
	}
	else if (input == 'c')
	{
		game.score += 100; 
	}
	else
	{
		std::cout << "Unknown input.\n";
	}
}

void UpdateGame(GameState& game)
{
	game.frame++; 

	if (game.playerHealth <= 0)
	{
		game.playerHealth = 0; 
		game.running = false;
	}
}

void RenderGame(const GameState& game)
{
	std::cout << "Frame: " << game.frame << "\n"; 
	std::cout << "Player X Position: " << game.playerX << "\n"; 
	std::cout << "Player Health: " << game.playerHealth << "\n"; 
	std::cout << "Score: " << game.score << "\n";
	std::cout << "----------------------\n";
}

int main()
{
	GameState game
	{
		true,
		0,
		0,
		100,
		0
	};

	while (game.running)
	{
		ProcessInput(game);
		UpdateGame(game);
		RenderGame(game); 
	}

	std::cout << "Game ended.\n";
	
	return 0;
}