/*
Lab 15: Constructors and Destructors

This exercise demonstrates how constructors and destructors define an object's lifecyle 
in C++

Core C++ concepts: 
	- default constructors: create an object with safe default values. 
	- parameterized constructors: create an object with specific starting values. 
	- member initializer lists: initialize member variables as the object is created.
	- destructors: run automatically when an object is destroyed.
	- scope: controls when stack objects are destroyed. 
	- const member functions: allow read-only status/output functions. 
	- std::vector::emplace_back: constructs objects directly inside a vector

Constructors define how an object begins its life. 

Examples: 
	Enemy unknownEnemy; 
	Enemy goblin("Goblin", 30, 5); 
	Projectile arrow("Arrow", 0.0f, 4,0f); 

A constructor hould put the object into a valid starting state.

Destructors define how an object ends its life. 

Examples: 
	~Enemy(); 
	~Projectile(); 

For stack objects, destructors run automatically when the object's scope ends. 
A scope is marked by braces { }.

Example: 
	{
		Projectile arrow("Arrow", 0.0f, 4.0f);
	} // arrow is destroyed here. 

This lab models an important game development idea: 
game objects have lifecycles. 

Examples: 
	- enemies spawn, act, take damage, and leave the world. 
	- projectiles spawn, move, become inactive, and are destroyed
	- temporary objects exist only inside a specific scope.
	- containers like std::vector own and destroy the objects they store. 

The Projectile class reinforces the lifecycle pattern: 
	- constructor sets initial position, velocity, and active state. 
	- Update changes the projectile over time
	- PrintStatus observes the current state
	- destructor marks the end of the projectile's lifetime.

This connects to larger C++ and engine concepts such as: 
	- object lifetime
	- valid initialization 
	- RAII
	- resource ownership
	- object pools
	- entity spawning
	- cleanup rules
	- vector reallocation and move/copy behavior. 

The main idea: 
constructors make sure objects start in a valid state, and destructors provide a predictable place
for cleanup or lifetime tracing when obejcts go out of scope. 
*/

#include <iostream>
#include <string>
#include <vector>

class Enemy
{
private: 
	std::string name; 
	int health; 
	int damage; 
	bool alive; 

public: 
	Enemy() 
		: name("Unknown Enemy"), health(10), damage(1), alive(true)
	{
		std::cout << "Default constructor: " << name << " spawned.\n";
	}

	Enemy(const std::string& enemyName, int enemyHealth, int enemyDamage)
		: name(enemyName), health(enemyHealth), damage(enemyDamage), alive(true)
	{
		std::cout << "Parameterized constructor: " << name << " spawned.\n"; 
	}

	~Enemy()
	{
		std::cout << "Destructor: " << name << " removed from the world.\n";
	}

	void Attack() const
	{
		if (!alive)
		{
			std::cout << name << " cannot attack because it is defeated.\n"; 
			return;
		}

		std::cout << name << " attacks for " << damage << " damage.\n";
	}

	void TakeDamage(int amount)
	{
		if (!alive)
		{
			std::cout << name << " is already defeated. Damage ignored";
			return;
		}

		health -= amount; 

		if (health <= 0)
		{
			health = 0; 
			alive = false;
			std::cout << name << " was defeated\n"; 
		}
		else
		{
			std::cout << name << " took " << amount << " damage. Health is now: " << health << "\n";
		}
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " | health: " << health
			<< " | damage: " << damage
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


void SpawnTemporaryEnemy()
{
	std::cout << "Entering SpawnTemporaryEnemy\n"; 

	Enemy slime("Slime", 15, 3); 
	slime.Attack(); 
	slime.TakeDamage(20); 
	slime.PrintStatus(); 

	std::cout << "Leaving SpawnTemporaryEnemy\n";
}

int main()
{
	std::cout << "Program started.\n"; 

	{
		Projectile arrow("Arrow", 0.0f, 4.0f);

		for (int i = 0; i < 3; ++i)
		{
			arrow.Update(1.0f);
			arrow.PrintStatus();
		}
	} // arrow is destroyed here

	std::cout << "Projectile scope ended.\n"; 

	Enemy unknownEnemy; 
	unknownEnemy.PrintStatus(); 

	std::cout << "----------------------\n";

	Enemy goblin("Goblin", 30, 5);
	goblin.Attack(); 
	goblin.TakeDamage(12);
	goblin.PrintStatus(); 

	std::cout << "----------------------\n";

	SpawnTemporaryEnemy(); 

	std::cout << "----------------------\n";

	std::vector<Enemy> enemies; 
	enemies.emplace_back("Skeleton", 25, 7); 
	enemies.emplace_back("Orc", 50, 12); 

	std::cout << "Enemies in vector:\n"; 

	for (const Enemy& enemy : enemies)
	{
		enemy.PrintStatus(); 
	}

	

	std::cout << "Program ended.\n"; 

	return 0;
	
}