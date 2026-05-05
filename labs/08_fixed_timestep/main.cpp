/*
Lab 08: Fixed Timestep 

This exercise demonstrates a fiex timestep game loop

A fixed timestep separates variable render frame timing from consistent simulation updates.

Core C++ concepts practiced: 
	- while loops: allows multiple simllation updates during one render frame. 
	- float accumulation: store elapsed time until enough time exists for an update. 
	- structs: groups entity data together 
	- references: allows FixedUpdate to modify real entities
	- const references: allows RenderEntity to read entities without modifying them 
	- std::vector: stores simulated frame times. 

Important terms: 
- frameTime: how long the current render frame took 
- fixedDeltaTime: the constant amount of time used for each simulation update
- accumulator: stores leftover elapsed time that has not yet been simulated 
- simulationStep: counts how many fixed updates have occured. 

The key idea: 
	accumulator += frameTime

Then, while enough time has accumulated: 
	while (accumulator >= fixedDeltaTime) 
	{
		FixedUpdate(entity, fixedDeltaTime); 
		accumulator -= fixedDeltaTime; 
	}

This means render frames can vary in length, but simulation always advances in consistent chunks, 
such as 1/60 of a second. 

This is different from normal delta time:
	- variable delta time uses the current frame's time directly 
	- fixed timestep uses the same fixedDeltaTime for evey simulator update

Why this matters: 
If a render frame is short, there may be no simulation update yet. 
If a render frame is long, there may be several simulation updates to catch up. 
The leftover time remains in the accumulator for the next frame. 

This makes simulation more stable and predictable, especially for systems like: 
- physics
- collision
- movement rules 
- deterministic replays 
- networked gameplay
- input prediction 

The player and enemy both update inside the fixed update loop, so they move 
according to fixed simulation steps rather than unpredictable render frame time.

The main idea: 
Rendering can happen at variable frame rates, but important simulation logic can run in fixed, 
consistent time steps.

*/

#include <iostream>
#include <vector>
#include <string>

struct Entity
{
	std::string name; 
	float x; 
	float velocity; 
};

void FixedUpdate(Entity& entity, float fixedDeltaTime)
{
	entity.x += entity.velocity * fixedDeltaTime;
}

void RenderEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | x: " << entity.x
		<< " | velocity: " << entity.velocity
		<< "\n"; 
}

int main()
{
	Entity player
	{
		"Player",
		0.0f,
		6.0f
	};

	Entity enemy
	{
		"Enemy",
		10.0f,
		-3.0f
	};

	const float fixedDeltaTime = 1.0f / 60.0f; 

	std::vector<float> frameTimes
	{
		0.010f,
		0.020f,
		0.050f,
		0.016f,
		0.033f
	}; 

	float accumulator = 0.0f; 
	int simulationStep = 0; 

	for (int frame = 0; frame < frameTimes.size(); ++frame)
	{
		float frameTime = frameTimes[frame];
		accumulator += frameTime; 

		std::cout << "Render Frame " << frame + 1 << "\n";
		std::cout << "Frame Time: " << frameTime << " seconds\n";
		std::cout << "Accumulator before updates: " << accumulator << "\n"; 

		while (accumulator >= fixedDeltaTime)
		{
			FixedUpdate(player, fixedDeltaTime);
			FixedUpdate(enemy, fixedDeltaTime);
			accumulator -= fixedDeltaTime; 
			simulationStep++; 

			std::cout << " Fixed Update Step " << simulationStep
				<< " | Player x: " << player.x
				<< " | Enemy x: " << enemy.x
				<< " | Remaining accumulator: " << accumulator
				<< "\n";
		}

		std::cout << "Rendered state:\n";
		RenderEntity(player);
		RenderEntity(enemy); 

		std::cout << "----------------------\n";
	}

	return 0; 
}