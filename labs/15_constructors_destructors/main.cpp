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
	enemies.emplace_back("Selecton", 25, 7); 
	enemies.emplace_back("Orc", 50, 12); 

	std::cout << "Enemies in vector:\n"; 

	for (const Enemy& enemy : enemies)
	{
		enemy.PrintStatus(); 
	}

	std::cout << "Program ended.\n"; 

	return 0;
	
}