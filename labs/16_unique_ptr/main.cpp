/*
Lab 16: std::unique_ptr 

This exercise demonstrates how to use std::unique_ptr to express exclusive ownership of heap allocated objects. 

Core C++ concepts practiced: 
	- std::unique_ptr: owns one heap-allocated object exclusively.
	- std::make_unique: safely creates heap objects managed by unique_ptr. 
	- automatic cleanup: owned objects are destroyed when the unique_ptr goes out of scope. 
	- std::vector<std::unique_ptr<T>>: stores and owns mulitple heap objects. 
	- pointer-style access: uses -> to call functions on the owned object. 
	- const references to unique_ptr: allows reading/updating owned objects in loops without copying ownership. 

Unlike raw new/delete, unique_ptr removes the need to manually delete objects. 

Example: 
	std::unique_ptr<Enemy> goblin = std::make_unique<Enemy>("Goblin", 30); 

When goblin goes out of scope, the Enemy is destroyed automatically. 

This lab models an important game development ownership pattern:
	A world, scene, manager, or system can own dynamically created game objects. 

Examples: 
	- GameWorld owned enemies. 
	- ProjectileSystem owns projectiles 
	- UIManager owns screens 
	- Scene owns scene objects 

A vector of unique_ptrs can own many objects: 
	std::vector<std::unique_ptr<Projectile>> projectiles;

Each unique_ptr owns one projectile. When the vector is destroyed, its unique_ptrs are destroyed, and each projectile is cleaned up automatically

This lab also reinforces valid initialization: 
Projectile initializes actve to true in the constructor so every projectile starts in a known state. 

The projectile update loop demonstrates a game-style system
	- projectiles are spawned
	- update over several frames 
	- marked inactive when they pass a boundary 
	- automatically destroyed when the owning container goes out of scope. 

This connects to larger C++ and engine concepts such as: 
	- ownership
	- RAII
	- heap lifetime management 
	- avoiding memory leaks 
	- avoiding manual delete 
	- move semantics
	- object managers 
	- resource managers
	- polymorphic game objects

The main idea: 
	Use std::unique_ptr when one owner is responsible for an object's lifetime, and let C++ clean up the object automatically when that owner goes away. 
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector> 

class Enemy
{
private:
	std::string name;
	int health;
	bool alive;

public:
	Enemy(const std::string& enemyName, int enemyHealth)
		: name(enemyName), health(enemyHealth), alive(true)
	{
		std::cout << "Enemy spawned: " << name << "\n";
	}

	~Enemy()
	{
		std::cout << "Enemy destroyed: " << name << "\n";
	}

	void TakeDamage(int amount)
	{
		if (!alive)
		{
			std::cout << name << " is already defeated. Damage ignored.\n";
			return;
		}

		health -= amount;

		if (health <= 0)
		{
			health = 0;
			alive = false;
			std::cout << name << " was defeated.\n";
		}
	}

	bool IsAlive() const
	{
		return alive;
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " | health: " << health
			<< " | alive: " << (alive ? "yes" : "no")
			<< "\n";
	}
};

class Projectile
{
private:
	std::string name;
	float x;
	float velocity;
	bool active;

public:
	Projectile(const std::string& projectileName, float projectileX, float projectileVelocity)
		: name(projectileName), x(projectileX), velocity(projectileVelocity), active(true)
	{
		std::cout << "Projectile spawned: " << name << "\n";
	}

	~Projectile()
	{
		std::cout << "Projectile destroyed: " << name << "\n";
	}

	void Update(float deltaTime)
	{
		if (!active)
		{
			return;
		}

		x += velocity * deltaTime;

		if (x >= 10.0f)
		{
			active = false;
		}
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " | x: " << x
			<< " | velocity: " << velocity
			<< " | active: " << (active ? "yes" : "no")
			<< "\n";
	}
};

void SingleOwnerExample()
{
	std::cout << "Entering SingleOwnerExample\n";

	std::unique_ptr<Enemy> goblin = std::make_unique<Enemy>("Goblin", 30);

	goblin->PrintStatus();
	goblin->TakeDamage(10);
	goblin->PrintStatus();

	std::cout << "Leaving SingleOwnerExample\n";
}

void WorldOwnsEnemiesExample()
{
	std::cout << "Entering WorldOwnsEnemiesExample\n";

	std::vector<std::unique_ptr<Enemy>> enemies;

	enemies.push_back(std::make_unique<Enemy>("Skeleton", 25));
	enemies.push_back(std::make_unique<Enemy>("Orc", 50));
	enemies.push_back(std::make_unique<Enemy>("Slime", 10));

	std::cout << "Enemies in world:\n";

	for (const std::unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->PrintStatus();
	}

	std::cout << "Damaging first enemy:\n";
	enemies[0]->TakeDamage(30);

	for (const std::unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->PrintStatus();
	}

	std::cout << "Leaving WorldOwnsEnemiesExample\n";
}

void WorldOwnsProjectilesExample()
{
	std::cout << "Entering WorldOwnsProjectilesExample\n";

	std::vector<std::unique_ptr<Projectile>> projectiles; 

	projectiles.push_back(std::make_unique<Projectile>("Arrow", 0.0f, 4.0f)); 
	projectiles.push_back(std::make_unique<Projectile>("Fireball", 2.0f, 6.0f)); 

	std::cout << "Projectiles in the world:\n";
	
	for(int frame = 1; frame <= 3; ++frame)
	{
		std::cout << "Projectile Frame " << frame << "\n";
		for (const std::unique_ptr<Projectile>& projectile : projectiles)
		{
			projectile->Update(1.0f);
			projectile->PrintStatus();
		}
	}

	std::cout << "Leaving WorldOwnsProjectilesExample\n";
}

int main()
{
	std::cout << "Program started\n";

	SingleOwnerExample();

	std::cout << "----------------------\n";

	WorldOwnsEnemiesExample();

	std::cout << "----------------------\n";

	WorldOwnsProjectilesExample();

	std::cout << "----------------------\n";

	std::cout << "Program ended.\n";

	return 0;
}
