/*
Lab 06: Multiple Entities 

This exercise introduces a simple world made of multiple entities stored in a std::vector. 

Core C++ concepts practiced: 
	- struct: groups common entity data together 
	- std::vector: stores a collection of entities 
	- range-based for loops: processes each entity in the collection 
	- references: allows functions to modify real entities instead of copies
	- const references: allows rendering to read entities without modifying them
	- bool flags: tracks whether an entity is alive or inactive

Each Entity stores: 
	- name 
	- x position
	- health
	- alive/dead status

This lab models an important game developmetn idea: 
	a game world usualyl contains many objects, and systems update those objects together. 

The update function loops through all entities and applies shared rules, while skipping entities that are no longer alive.
This introduces the idea of systems processing collections of objects. 

This connects to larger game and engine concepts such as: 
	- World / entity lists
	- Movement systems
	- Combat and damage systems
	- Alive / Dead state
	- Object lifetime 
	- Entity-component systems 
	- Batch processing 
	- Data-oriented design.

The main idea:
instead of riting one-off logic for individual objects, store objects in a collection and use systems to update them consistently over time.
*/

#include <iostream>
#include <string>
#include <vector>

struct Entity
{
	std::string name; 
	int x; 
	int health; 
	bool alive; 
};

void DamageEntity(Entity& entity, int amount)
{
	if (!entity.alive)
	{
		return; 
	}

	entity.health -= amount; 

	if (entity.health <= 0)
	{
		entity.health = 0; 
		entity.alive = false; 
		std::cout << entity.name << " was defeated.\n"; 
	}
}

void MoveEntity(Entity& entity, int amount)
{
	if (!entity.alive)
	{
		return;
	}

	entity.x += amount; 
}

void UpdateEntities(std::vector<Entity>& entities)
{
	for (Entity& entity : entities)
	{
		if (!entity.alive)
		{
			continue; 
		}
		if (entity.name == "Goblin")
		{
			MoveEntity(entity, -1); 
		}
		if (entity.name == "Skeleton")
		{
			MoveEntity(entity, -2);
		}
		else if (entity.name == "Player")
		{
			MoveEntity(entity, 1); 
		}
	}
}

void RenderEntities(const std::vector<Entity>& entities)
{
	for (const Entity& entity : entities)
	{
		std::cout << entity.name
			<< " | x: " << entity.x
			<< " | health: " << entity.health
			<< " | alive: " << (entity.alive ? "yes" : "no")
			<< "\n";
	}

	std::cout << "----------------------\n";
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 0, 100, true},
		{"Goblin", 5, 30, true},
		{"Slime", 8, 20, true},
		{"Skeleton", 12, 40, true}
	};

	for (int frame = 1; frame <= 5; ++frame)
	{
		std::cout << "Frame " << frame << "\n";

		if (frame == 2)
		{
			DamageEntity(entities[1], 15);
		}
		if (frame == 3)
		{
			DamageEntity(entities[1], 20); 
		}
		if (frame == 4)
		{
			DamageEntity(entities[2], 25); 
		}

		UpdateEntities(entities); 
		RenderEntities(entities);
	}

	return 0; 
}
