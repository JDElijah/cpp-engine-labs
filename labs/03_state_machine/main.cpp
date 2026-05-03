/*
Lab 03: State Machine

This exercise demonstrates a basic finite state mahine for enemy behavior. 

Core C++ concepts practiced: 
- enum class: defines a limited set of valid states
- switch statements: converts state values into readable output 
- struct: groups enemy data together 
- references: allows UpdateEnemy to modify the actual enemy object 
- functions: separates state updating from rendering/output 

The EnemyState enum represents the enemy's possible behavior modes: 
	- Idle 
	- Patrol
	- Chase
	- Attack
	- Flee
	- Dead 

The Enemy struct stores the data needed to make behavior decisions: 
- name: identity/debug label 
- state: current behavior mode 
- health: determines whether the enemy should flee or die 
- distanceToPlayer: determines whether the enemy should patrol, chase, or attack. 

The UpdateEnemy function contains the transition rules. 
It changes the enemy's state based on its current data. 

Example transition priority: 
	health <= 0      -> Dead
	health <= 20     -> Flee
	distance <= 2    -> Attack
	distance <= 8    -> Chase
	otherwise        -> Patrol

Order matters. Dead must be checked before Flee, beacuse an enemy with 0 health should not
flee. Higher-priority states should override lower-priority behavior. 

This models an important game develop idea: 
Objects behave differenly depending on their current state and conditions. 

State mahines are commonly used for: 
- enemy Ai 
- player movement states 
- animation systems 
- menus 
- boss phases 
- weapon behavior 
- game modes 

This lab connects to larger systems thingkingby showing how behavior can be modeled
explicity instead of becoming a tangled collection of unrealed if-statements. 

The main idea: 
a state mahine organizes behavior into clear modes and defines the rules for mooving 
between two modes. 
*/

#include <iostream>
#include <string>

enum class EnemyState
{
	Idle,
	Patrol,
	Chase,
	Attack,
	Flee
};

struct Enemy
{
	std::string name;
	EnemyState state;
	int health;
	float distanceToPlayer;
};

std::string StateToString(EnemyState state)
{
	switch (state)
	{
	case EnemyState::Idle:
		return "Idle";
	case EnemyState::Patrol:
		return "Patrol"; 
	case EnemyState::Chase:
		return "Chase"; 
	case EnemyState::Attack: 
		return "Attack"; 
	case EnemyState::Flee:
		return "Flee";  
	default:
		return "Unknown"; 
	}
}

void UpdateEnemy(Enemy& enemy)
{
	if (enemy.health <= 20)
	{
		enemy.state = EnemyState::Flee;
	}
	else if (enemy.distanceToPlayer <= 2.0f)
	{
		enemy.state = EnemyState::Attack; 
	}
	else if (enemy.distanceToPlayer <= 8.0f)
	{
		enemy.state = EnemyState::Chase;
	}
	else
	{
		enemy.state = EnemyState::Patrol; 
	}
}

void RenderEnemy(const Enemy& enemy)
{
	std::cout << enemy.name << "\n"; 
	std::cout << "Health: " << enemy.health << "\n"; 
	std::cout << "Distance to Player: " << enemy.distanceToPlayer << "\n"; 
	std::cout << "State: " << StateToString(enemy.state) << "\n"; 
	std::cout << "----------------------\n";
}

int main()
{
	Enemy goblin
	{
		"Goblin",
		EnemyState::Idle,
		100,
		12.0f
	};

	for (int frame = 1; frame <= 6; ++frame)
	{
		std::cout << "Frame " << frame << "\n";
		if (frame == 2)
		{
			goblin.distanceToPlayer = 7.0f; 
		}
		else if (frame == 3)
		{
			goblin.distanceToPlayer = 1.5f; 
		}
		else if (frame == 4)
		{
			goblin.health = 15; 
		}
		else if (frame == 5)
		{
			goblin.distanceToPlayer = 10.0f; 
		}
		UpdateEnemy(goblin); 
		RenderEnemy(goblin); 
	}
	return 0; 
}