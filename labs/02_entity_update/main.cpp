/*
Lab 02: Entity Update

This exercise demonstrates how to update multiple game objects or entities, inside a simple simulation loop. 

Core C++ concepts practiced: 
- struct: groups related data into one type
- std::vector: stores a collection of entities 
- references: allows functions and loops to modify real objects 
- const references: allows read-only access without copying 
- functions: separates update logic from render/output logic. 

The Entity struct represents a simple object in the world: 
- name: identity/debug label 
- x: position 
- velocity: movement speed and direction 

Each frame, every entity is updated with: 
	position = position + velocity * deltaTime

This models a basic movement simulation. 

The Update function takes Entity& becuase it needs to modify the actual entity. 
Without&*, the function or loop could workd on a copy instead, and changes might not persist. 

The Render function takes const Entity& because it only reads the entity's data
and prints it. This avoids unnecessary copying whle precenting accidental chagnes. 

The boundary check introduces simple contraint/collision-style logic: 
if an entity movespart the left or right boundary, its velocity is reversed, causing it to bounce. 

This mirrors an important game engine patter: 

	for each entity: 
		update simulation state
		render/output current state

This small lab connects to larger game development and sysmts concepts such as: 
- entity systems 
- component systems 
- physics systems 
- collison response 
- movement simulation 
- data-oriented thinking
- proessing many objects with shared rules 

The main idea: 
A game world is a collection of objects whose state changes over time according to shared update rules. 

*/

#include <iostream>
#include <string>
#include <vector>

struct Entity
{
	std::string name;
	float x; 
	float velocity; 
};

void Update(Entity& entity, float deltaTime)
{
	entity.x += entity.velocity * deltaTime;

	if (entity.x >= 10.0f)
	{
		entity.velocity = -entity.velocity;
	}
	else if (entity.x <= 0.0f)
	{
		entity.velocity = -entity.velocity;
	}
}

void Render(const Entity& entity)
{
	std::cout
		<< entity.name
		<< " position: "
		<< entity.x
		<< "\n";
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 0.0f, 2.0f},
		{"Enemy", 10.0f, -1.0f},
		{"NPC", 5.0f, 0.5f}
	};

	const float deltaTime = 1.0f; 

	for (int frame = 1; frame <= 5; ++frame)
	{
		std::cout << "Frame " << frame << "\n";

		for (Entity& entity : entities)
		{
			Update(entity, deltaTime);
			Render(entity);
		}

		std::cout << "----------------------\n";

	}

	return 0; 
}