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

	bool isAlive() const
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

int main()
{
	std::cout << "Program started\n";

	SingleOwnerExample(); 

	std::cout << "----------------------\n";

	WorldOwnsEnemiesExample(); 

	std::cout << "----------------------\n";

	std::cout << "Program ended.\n"; 

	return 0;
}
