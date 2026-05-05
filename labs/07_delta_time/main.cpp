/*
Lab 07: Delta Time 

This exercise demonstrates how to update movemetn and other time-based values
using delta time. 

Delta time is the amound of time that passed since the previous frame. 

Core C++ concepts practiced: 
	- float values for time, position, velocity, and stamina
	- struct: groups entity data together 
	- std::vector: stores multiple entities and simulated frame times
	- references: allows update functions to modify real entities 
	- const references: allows rendering to read entities without modifying them
	- basic simulation math. 

The key formula is: 
	amount changed = rate * deltaTime

For movement: 
	position += velocity * deltaTime

For stamina regeneration: 
	staimina += regenerationRate * deltaTime

This makes behavior based on real time instead of frame count. Without deltaTime, 
faster computers or higher frame rates could cause objects to move or change faster.

This lab models an important game development idea:
time-based behavior should usually be expressed as rates per second. 

Examples: 
	- units moved per second
	- stamina regenerated per second
	- cooldown seconds remaining
	- animation time advanced per second

The stamina logic also introduces clamping: 
after increasing stamina, it is limited to a maximum value of 100

This connects to larger engine concepts such as: 
	- frame-rate-independent movement 
	- real-time simulation
	- timers and cooldowns
	- animation updates
	- particle systems
	- Unreal's Tick(float DeltaTime) 

The main idea: 
use delta time to make simulation behavior consistent across different frame rates
and hardware speeds. 
*/

#include <iostream>
#include <vector>
#include <string>

struct Entity
{
	std::string name; 
	float x; 
	float velocity;
	float stamina; 
};

void RegenerateStamina(Entity& entity, float deltaTime)
{
	entity.stamina += 5.0f * deltaTime; 
	if (entity.stamina > 100.0f)
	{
		entity.stamina = 100.0f; 
	}
}

void UpdateEntity(Entity& entity, float deltaTime)
{
	entity.x += entity.velocity * deltaTime; 
}

void RenderEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | x: " << entity.x
		<< " | velocity: " << entity.velocity
		<< " | stamina: " << entity.stamina
		<< "\n";
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 0.0f, 4.0f, 50.0f},
		{"Enemy", 10.0f, -2.0f, 0.0f}
	};

	std::vector<float> frameTimes
	{
		0.016f, // about 60 FPS
		0.016f,
		0.033f, // about 30 FPS
		0.010f, // abou 100 FPS
		0.025f
	}; 

	float totalTime = 0.0f; 
	
	for (int frame = 0; frame < frameTimes.size(); ++frame)
	{
		float deltaTime = frameTimes[frame]; 
		totalTime += deltaTime; 

		std::cout << "Frame " << frame + 1 << "\n"; 
		std::cout << "Delta Time: " << deltaTime << " seconds\n"; 
		std::cout << "Total Time: " << totalTime << " seconds\n";

		for (Entity& entity : entities) 
		{
			if (entity.name == "Player")
			{
				RegenerateStamina(entity, deltaTime); 
			}
			UpdateEntity(entity, deltaTime);
			RenderEntity(entity); 
		}

		std::cout << "----------------------\n";
	}

	return 0; 
}