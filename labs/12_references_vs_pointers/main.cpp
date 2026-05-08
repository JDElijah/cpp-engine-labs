/*
Lab 12: References vs Pointers

This exercise demonstrates the difference between using references and pointers 
to access existing objects without copying them. 

Core C++ concepts practiced: 
	- references: use when a function requires a valid object. 
	- const references: use when reading an object without modifying it. 
	- pointers: use when an object may or may not exist.
	- nullptr: represents "no object"
	- address-of operator &: gets teh address of an object 
	- dereference operator *: access theobject a pointer points to

References are useful when the target must exist. 

Example: 
	ApplyDamage(goblin, 15); 

ApplyDamage takes Entity& because it needs to modify the real goblin, not a copy. 

Const references are useful for read-only access. 

Example: 
	PrintEntity(const Entity& entity); 

PrintEntity can inspect the entity without copying it and without changing it.

Pointers are useful for optional targets. 

Example: 
	Entity* selectedTarget = nullptr; 

A selected target, selected ally, equiped item, owener, or focused UI element may exist,
or it may be missing. A pointer can represent either case. 

Before using a pointer, check whether it is nullptr. 

Example: 
	if (target == nullptr) 
	{
		return; 
	}

Then dereference it when passing to a function that expects a reference. 

Example: 
	Heal(*target, amount) 

This lab models an important game development idea: 
some systems require an object, while other systems must safely handle the absence of an object. 

This connects to a larger C++ and engine concepts such as: 
- object identity
- optional relationships 
- target selection
- ownership vs access
- dangling pointer risks
- entity managers 
- object lifetime 

The main idea: 
use references when an object is required, and use pointers when an object is optional or may be missing. 
*/

#include <iostream>	
#include <string>

struct Entity
{
	std::string name;
	int health;
	bool alive;
};

void PrintEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | health: " << entity.health
		<< " | alive: " << (entity.alive ? "yes" : "no")
		<< "\n";
}

void ApplyDamage(Entity& target, int amount)
{
	if (!target.alive)
	{
		std::cout << target.name << " is alread deafeated. Damage ignored.\n";
		return; 
	}

	target.health -= amount;
	if (target.health <= 0)
	{
		target.health = 0;
		target.alive = false; 
		std::cout << target.name << " was defeated.\n";
	}
}

void Heal(Entity& target, int amount)
{
	if (!target.alive)
	{
		std::cout << target.name << " cannot be healed because they are defeated.\n";
		return;
	}

	target.health += amount; 

	if (target.health > 100)
	{
		target.health = 100; 
	}
}

void AttackOptionalTarget(Entity* target, int amount)
{
	if (target == nullptr)
	{
		std::cout << "No target selected. Attack cancelled.\n";
		return;
	}
	ApplyDamage(*target, amount); 
}

void HealOptionalTarget(Entity* target, int amount)
{
	if (target == nullptr)
	{
		std::cout << "No target selected. Heal cancelled.\n";
		return;
	}
	Heal(*target, amount); 
}

Entity* ChooseTarget(Entity& enemy, bool playerCanSeeEnemy)
{
	if (playerCanSeeEnemy && enemy.alive) 
	{
		return &enemy; 
	}

	return nullptr; 
}

int main()
{
	Entity player
	{
		"Player",
		100,
		true
	};

	Entity goblin
	{
		"Goblin",
		40,
		true
	};

	std::cout << "Initial state:\n";
	PrintEntity(player); 
	PrintEntity(goblin);
	std::cout << "----------------------\n";

	std::cout << "Damage using reference:\n"; 
	ApplyDamage(goblin, 15); 
	PrintEntity(goblin); 
	std::cout << "----------------------\n";

	std::cout << "Heal using reference:\n";
	Heal(player, 10); 
	PrintEntity(player); 
	std::cout << "----------------------\n";

	std::cout << "Attack optional target using pointer:\n"; 
	Entity* selectedTarget = ChooseTarget(goblin, true); 
	AttackOptionalTarget(selectedTarget, 30); 
	PrintEntity(goblin); 
	std::cout << "----------------------\n";

	std::cout << "Try attacking with no selected target:\n";
	selectedTarget = ChooseTarget(goblin, false); 
	AttackOptionalTarget(selectedTarget, 30); 
	std::cout << "----------------------\n";

	Entity* selectedAlly = &player; 
	HealOptionalTarget(selectedAlly, 20);
	PrintEntity(player); 
	std::cout << "----------------------\n";

	selectedAlly = nullptr; 
	HealOptionalTarget(selectedAlly, 20); 
	std::cout << "----------------------\n";

	return 0; 
}