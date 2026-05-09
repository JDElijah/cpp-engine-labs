/*
Lab 13: Const Correctness

This exercise demonsetrats how to use const to separate read-only code from code that modifies game state. 

Core C++ concepts practiced: 
- const references: allows functions to read objects without copying or changing them. 
- const member functions: allows query functions to be called on const objects. 
- references: allows mutation functions to modify real objects 
- static_cast<float>: converts integers to floats for percentage calculations
- std::vector: stores multiple entities. 

Const member functions promise not to modify the object. 

Examples: 
	bool IsAlive() const; 
	float GetHealthPercent() const; 
	bool HasFullStamina() const; 

These fucntions only inspect the entity's state. 

Read-only functions should take const references. 

Examples: 
	void PrintEntity(const Entity& entity); 
	void PrintWorld(const std::vector<Entity>& entities); 
	void PrintStaminaStatus(const Entity& entity); 
	bool CanAttack(const Entity& entity); 

These functions observe game state but shoud not change it. 

Mutation functions should take non-const references. 

Examples: 
	void ApplyDamage(Entity& entity, int amount); 
	void UseStamina(Entity& entity, int amount); 
	void TryAttack(Entity& attacker, Entity& target); 

These functions intentially change game state. 

This lab models an important game development idea: 
some systems should only observe state, while other systems are allowed to modify it. 

Examples: 
	- rendering reads position, health, stamina, and alive status. 
	- UI reads values to display bars and status text. 
	- AI checks whether an entity can attack
	- Gameplay actions apply damage, spend stamina, or change state. 

Const correctness helps prevent accidental side effects. A render, print, or query function should 
not accidentally modify the simulation. 

This connects to larger systems concepts such as: 
- read-only access
- API safety
- Side-effect control
- Ownership boundaries
- Debugging
- Testing 
- Thread safey
- Reviewing AI-generated code for unsafe mutation

The main idea: 
Use const to make function intent clear: read-only code should be protected from changing state, while mutation code should be explicit. 


*/

#include <iostream>
#include <string>
#include <vector>	

struct Entity
{
	std::string name; 
	int health; 
	int maxHealth; 
	int stamina;
	int maxStamina; 
	bool alive; 

	bool IsAlive() const
	{
		return alive; 
	}

	float GetHealthPercent() const
	{
		return (static_cast<float>(health) / maxHealth) * 100; 
	}

	bool HasFullStamina() const
	{
		return stamina == maxStamina; 
	}
};

void PrintEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | health: " << entity.health << "/" << entity.maxHealth
		<< " | stamina: " << entity.stamina << "/" << entity.maxStamina
		<< " | alive: " << (entity.IsAlive() ? "yes" : "no")
		<< " | health percent: " << entity.GetHealthPercent()
		<< "\n"; 
}

void PrintWorld(const std::vector<Entity>& entities)
{
	std::cout << "World State: \n"; 

	for (const Entity& entity : entities)
	{
		PrintEntity(entity); 
	}

	std::cout << "----------------------\n";
}

void PrintStaminaStatus(const Entity& entity)
{
	if (!entity.IsAlive())
	{
		std::cout << entity.name << " is already defeated. Stamina is 0.\n";
		return;
	}
	else if (entity.HasFullStamina())
	{
		std::cout << entity.name << " has full stamina. [" << entity.stamina << "]\n"; 
		return;
	}

	std::cout << entity.name << " does not have full stamina. [" << entity.stamina << "]\n";

}


void ApplyDamage(Entity& entity, int amount)
{
	if (!entity.IsAlive())
	{
		std::cout << entity.name << " is already defeated. Damage ignored.\n"; 
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

void UseStamina(Entity& entity, int amount)
{
	entity.stamina -= amount; 

	if (entity.stamina < 0)
	{
		entity.stamina = 0; 
	}
}

bool CanAttack(const Entity& entity)
{
	return entity.IsAlive() && entity.stamina >= 10; 
}

void TryAttack(Entity& attacker, Entity& target)
{
	if (!CanAttack(attacker))
	{
		std::cout << attacker.name << " cannot attack.\n"; 
		return;
	}

	std::cout << attacker.name << " attacks " << target.name << "!\n"; 

	UseStamina(attacker, 10); 
	ApplyDamage(target, 15); 
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 100, 100, 30, 100, true},
		{"Goblin", 40, 40, 10, 30, true}
	}; 

	PrintWorld(entities); 

	PrintStaminaStatus(entities[0]);
	PrintStaminaStatus(entities[1]);

	TryAttack(entities[0], entities[1]); 

	PrintStaminaStatus(entities[0]);
	PrintStaminaStatus(entities[1]);

	PrintWorld(entities); 

	PrintStaminaStatus(entities[0]);
	PrintStaminaStatus(entities[1]);

	TryAttack(entities[1], entities[0]); 

	PrintStaminaStatus(entities[0]);
	PrintStaminaStatus(entities[1]);

	PrintWorld(entities);

	return 0; 
}




