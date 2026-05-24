/* 
Lab 20: Object Lifetime Bugs

This exercise demonstrates how pointers and references can become unsafe when the object they refer to is 
destroyed, removed, or moved in memory. 

Core C++ concepts practiced: 
	- object lifetime: every object exists only for a specific period of time. 
	- scope: stack objects are destroyed whne their scope ends. 
	- dangling pointer: a pointer that sill stores an address after the object is gone. 
	- dangling reference: a reference to an object that no longer exists or is no longer valid 
	- vector reallocation: std::vector may move elements when it grows. 
	- erase invalidation: removing elements from a vector can invalidate pointers, references, and iterators. 
	- nullptr checks: used to safely handle failed lookups
	- ID lookup: storing an object ID instead of a long-lived raw pointer. 

The main unsafe pattern demonstrated: 
	a pointer can outlive the object it points to. 

Example: 
	Enemy* target = &goblin; 

If goblin is destroyed when its scope ends, target does not automatically become nullptr.
It still contains an old address, but the aobject no longer exists. Using that pointer 
would be a dangling pointer bug. 

The safe pattern demonstrated: 
	Store long-term identity as an ID, then ask the World if the object still exists. 

Example: 
	int targetId = 1; 
		Enemy* target = world.FindEnemyById(targetId);

The returned pointer should be treated as temporary.It is safe only while the World
has not removed, erased, or relocated the enemy. After the World changes, look up 
the enemy again by ID and check for nullptr. 

The TargetTracker challenge reinforces this idea: 
	- TargetTracker stores targetId, not Enemy*
	- PrintTargetStatus asks the World to find the current enemy
	- If the enemy exists, it prints the enemy status 
	- If the enemy was removed, FindEnemyById returns nullptr 
	- no dangerous long-lived pointer is kept inside the tracker

This models common game development problems: 
	- AI targeting an enemy that was destroyed. 
	- UI pointing to an inventory item that was removed 
	- Projectiles tracking a target that despawned. 
	- cameras following entities that no longer exist. 
	- multiplayer clients referencing objects the server deleted. 

This connects to larger systems concepts such as: 
	- ownership 
	- lifetime management 
	- world/entity managers
	- entity IDs and handles
	- weak references
	- safe lookup patterns
	- avoiding stale references 
	- preventing hard-to-debug crashes

The main idea: 
	A pointer or reference is only safe if the object it refers to is still alive 
	and has not been invalidated. For long-term tracking, store an ID or handle and look
	up the object when needed instead of keeping a raw pointer forever. 
*/

#include <iostream>
#include <string> 
#include <vector>

class Enemy
{
private:
	int id; 
	std::string name; 
	int health; 
	bool alive; 

public:
	Enemy(int enemyId, const std::string& enemyName, int enemyHealth)
		: id(enemyId), name(enemyName), health(enemyHealth), alive(true)
	{
		std::cout << "Enemy created: " << name << " [id: " << id << "]\n";
	}

	Enemy(const Enemy& other)
		: id(other.id), name(other.name), health(other.health), alive(other.alive)
	{
		std::cout << "Enemy copied: " << name << " [id: " << id << "]\n";
	}

	Enemy(Enemy&& other) noexcept
		: id(other.id), name(other.name), health(other.health), alive(other.alive)
	{
		std::cout << "Enemy moved: " << name << " [id: " << id << "]\n";
		
		other.id = -1; 
		other.health = 0; 
		other.alive = false; 
	}

	Enemy& operator=(const Enemy& other)
	{
		if (this != &other) // In this assignment, we are not asking whether the two objects have the same value; 
			// we are asking whether the source and destination are the same object. Same object means the same
			// address, so compare addresses. This line: address of destination != address of source. 
		{
			id = other.id;
			name = other.name;
			health = other.health; 
			alive = other.alive; 

			std::cout << "Enemy copy-assigned: " << name << " [id: " << id << "]\n";
		}
		
		return *this; 
	}

	Enemy& operator=(Enemy&& other) noexcept
	{
		if (this != &other)
		{
			id = other.id; 
			name = other.name;
			health = other.health; 
			alive = other.alive; 

			other.id = -1; 
			other.health = 0; 
			other.alive = false;
			
			std::cout << "Enemy move-assigned: " << name << " [id: " << id << "]\n";
		}

		return *this; 
	}

	~Enemy()
	{
		std::cout << "Enemy destroyed: " << name << " [id: " << id << "]\n";
	}

	int GetId() const
	{
		return id; 
	}

	const std::string& GetName() const // Add const in front of the return type because the function is returning a reference to internal data that should not be modified.  
	{
		return name; 
	}

	bool IsAlive() const
	{
		return alive; 
	}

	void TakeDamage(int amount)
	{
		if (!alive)
		{
			return;
		}
		
		std::cout << name << " damaged for " << amount << "!\n";

		health -= amount; 

		if (health <= 0)
		{
			health = 0; 
			alive = false; 
			std::cout << name << " defeated.\n";
		}
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " [id: " << id << "]"
			<< " | health: " << health
			<< " | alive: " << std::boolalpha << alive
			<< "\n";
	}
};

class World
{
private:
	std::vector<Enemy> enemies; 

public: 
	void SpawnEnemy(int id, const std::string& name, int health)
	{
		enemies.emplace_back(id, name, health); 
	}

	Enemy* FindEnemyById(int id)
	{
		for (Enemy& enemy : enemies)
		{
			if (enemy.GetId() == id)
			{
				return &enemy; 
			}
		}

		return nullptr; 
	}

	void RemoveDeadEnemies()
	{
		for (std::size_t i = 0; i < enemies.size();)
		{
			if (!enemies[i].IsAlive())
			{
				std::cout << "Removing dead enemy: " << enemies[i].GetName() << "\n";
				enemies.erase(enemies.begin() + i);
			}
			else
			{
				i++; 
			}
		}
	}

	void PrintWorld() const
	{
		std::cout << "World enemies:\n"; 

		if (enemies.empty())
		{
			std::cout << "   None\n";
			return;
		}

		for (const Enemy& enemy : enemies)
		{
			std::cout << "  ";
			enemy.PrintStatus(); 
		}
	}
};

class TargetTracker
{
private:
	int targetId; 

public: 
	TargetTracker()
		: targetId(-1)
	{
	}

	void SetTarget(int id)
	{
		targetId = id; 
	}

	void ClearTarget()
	{
		targetId = -1;
	}

	void PrintTargetStatus(World& world) const
	{
		if (targetId == -1)
		{
			std::cout << "No target selected.\n"; 
			return;
		}

		Enemy* targetEnemy = world.FindEnemyById(targetId); 

		if (targetEnemy != nullptr)
		{
			std::cout << "Target found:\n";
			targetEnemy->PrintStatus();
		}
		else
		{
			std::cout << "Target no longer exists.\n";
		}
	}
};

void UnsafePointerExample()
{
	std::cout << "\n=== Unsafe pointer example ===\n";

	Enemy* target = nullptr;

	{
		Enemy goblin(1, "Goblin", 30); 
		target = &goblin; 

		std::cout << "Inside scope:\n";
		target->PrintStatus(); 
	}

	std::cout << "After scope ended:\n"; 
	std::cout << "target now points to an object that no longer exists.\n"; 

	// Do NOT do this:
	// target->PrintStatus(); 

	std::cout << "Accessing target here would be a danglining pointer.\n";
}

void SaferIdLookupExample()
{
	std::cout << "\n=== Safer ID lookup example ===\n";

	World world;

	world.SpawnEnemy(1, "Goblin", 30); 
	world.SpawnEnemy(2, "Skeleton", 40); 

	int targetId = 1; 

	std::cout << "Target selected by id: " << targetId << "\n";

	Enemy* target = world.FindEnemyById(targetId); 

	if (target != nullptr)
	{
		std::cout << "Target found:\n";
		target->PrintStatus(); 

		std::cout << "Damaging target:\n"; 
		target->TakeDamage(999);
	}

	world.RemoveDeadEnemies(); 

	std::cout << "Trying to find target again after removal:\n";
	target = world.FindEnemyById(targetId);

	if (target != nullptr)
	{
		target->PrintStatus(); 
	}
	else
	{
		std::cout << "Target no longer exists.\n"; 
	}
	world.PrintWorld(); 
}

void VectorReferenceRiskExample()
{
	std::cout << "\n=== Vector reference risk example ===\n";

	std::vector<Enemy> enemies; 

	enemies.reserve(1); 

	enemies.emplace_back(1, "Goblin", 30); 

	Enemy& selectEnemy = enemies[0]; 

	std::cout << "Selected enemy before vector grows:\n";
	selectEnemy.PrintStatus(); 

	std::cout << "Adding another enemy may cause vector reallocation.\n"; 
	enemies.emplace_back(2, "Orc", 50); 

	std::cout << "After vector groth, old references may be invalid.\n"; 

	// Do NOT rely on this after possible vector reallocation: 
	// selectedEnemy.PrintStatus()

	std::cout << "Instead, look the enemy up again by index or id.\n"; 
	enemies[0].PrintStatus();
}

int main()
{
	std::cout << "Program started\n"; 

	UnsafePointerExample(); 

	std::cout << "----------------------\n";

	SaferIdLookupExample(); 

	std::cout << "----------------------\n";

	VectorReferenceRiskExample();

	std::cout << "----------------------\n";

	std::cout << "\n=== Target Tracker Example ===\n";

	World world;
	world.SpawnEnemy(1, "Goblin", 50);
	world.SpawnEnemy(2, "Skeleton", 80); 

	TargetTracker targetTracker; 
	targetTracker.SetTarget(1); 

	targetTracker.PrintTargetStatus(world);
	
	Enemy* goblin = world.FindEnemyById(1);

	if (goblin != nullptr)
	{
		goblin->TakeDamage(50); 
	}

	world.RemoveDeadEnemies(); 

	targetTracker.PrintTargetStatus(world);

	std::cout << "----------------------\n";

	std::cout << "Program ended\n"; 

	return 0;
}

