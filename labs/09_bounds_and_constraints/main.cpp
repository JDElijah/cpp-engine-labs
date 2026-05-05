/*
Lab 09: Bounds and Constraints

This exercise demonstrates how to keep game state inside valid limits.

Core C++ concepts practiced: 
	- functions: centralizes rules like clamping, damage, healing, and movement. 
	- structs: grous entity data together 
	- references: allows functions to modify real entities 
	- const references: allows rendering to read entities without modifying them
	- std::vector: stores multiple entities
	- conditionals: checks whether values are outside valid ranges. 

Each Entity stores: 
	- name
	- x position 
	- velocity 
	- health
	- stamina 
	- minimum and maximum movement bounds

This lab introducting constraints: 
rules that prevent values from becoming invalid. 

Examples: 
- health should stay between 0 and 100 
- stamina should stay between 0 and 1000 
- position should stay between minX and maxX

This lab introduces constraints: 
rules that prevent values from becoming invalid. 

Examples: 
- health should stay between 0 and 100
- stamina should stay between 0 and 100
- position should stay between minX and maxX

Clamping keeps values inside a valid range: 
	value < min -> use min
	value > max -> use max
	otherwise -> keep value

Movement uses bounds as a simple constraint system: 
the entity moves, then if it passes minX and maxX, its position is corrected
and its velocity is reversed. 

This models an important game development idea: 
game state should be protected from impossible or invalid values. 

This connects to larger systems concepts such as; 
	- invariants
	- data validation 
	- collision constraints
	- patrol bounds
	- resource limits
	- simulation correctness
	- preventing bugs caused by invalid state 

The main idea: 
when a system changes game state, it should also preserve the rules that keep that
state valid. 
*/

#include <iostream>
#include <string>
#include <vector>

struct Entity
{
	std::string name; 
	float x; 
	float velocity; 
	int health; 
	int stamina; 
	float minX; 
	float maxX;
};

float ClampFloat(float value, float minValue, float maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

float ClampInt(int value, int minValue, int maxValue)
{
	if (value < minValue)
	{
		return minValue; 
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value; 
}

void ApplyDamage(Entity& entity, int amount)
{
	entity.health -= amount;
	entity.health = ClampInt(entity.health, 0, 100);
}

void Heal(Entity& entity, int amount)
{
	entity.health += amount; 
	entity.health = ClampInt(entity.health, 0, 100); 
}

void RegenStamina(Entity& entity, int amount)
{
	entity.stamina += amount;
	entity.stamina = ClampInt(entity.stamina, 0, 100);
}

void UseStamina(Entity& entity, int amount)
{
	entity.stamina -= amount; 
	entity.stamina = ClampInt(entity.stamina, 0, 100); 
}

void MoveEntity(Entity& entity, float deltaTime)
{
	entity.x += entity.velocity * deltaTime; 

	if (entity.x < entity.minX)
	{
		entity.x = entity.minX;
		entity.velocity = -entity.velocity; 
	}
	else if (entity.x > entity.maxX)
	{
		entity.x = entity.maxX;
		entity.velocity = -entity.velocity;
	}
}

void RenderEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | X: " << entity.x
		<< " | velocity: " << entity.velocity
		<< " | health: " << entity.health
		<< " | stamina: " << entity.stamina
		<< " | bounds: [" << entity.minX << ", " << entity.maxX << "]"
		<< "\n";
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 0.0f, 4.0f, 100, 50, 0.0f, 10.0f},
		{"Goblin", 8.0f, 3.0f, 30, 20, 2.0f, 9.0f}
	};

	const float deltaTime = 1.0f;

	for (int frame = 1; frame <= 6; ++frame)
	{
		std::cout << "Frame " << frame << "\n"; 
		
		if (frame == 2)
		{
			ApplyDamage(entities[0], 35);
			UseStamina(entities[0], 20);
		}
		if (frame == 3)
		{
			ApplyDamage(entities[0], 90);
			UseStamina(entities[0], 50); 
		}
		if (frame == 4)
		{
			Heal(entities[0], 50); 
			RegenStamina(entities[0], 80); 
		}
		for (Entity& entity : entities)
		{
			MoveEntity(entity, deltaTime); 
			RenderEntity(entity);
		}

		std::cout << "----------------------\n";

	}

	return 0; 
}