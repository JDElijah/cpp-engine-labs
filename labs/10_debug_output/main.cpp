/*
Lab 10: Debug Output

This exercise demonstrates how to use structured console output to make game 
state changes easier to understand and debug. 

Core C++ concepts practiced: 
	- enum class: defines log levels such as Info, Warning, and Error. 
	- switch statements: converts log levels into readble texts. 
	- functions: centralizes logging, damage, healing, stamina use, and rendering
	- references: allows gameplay functions to modify real entities 
	- const references: allwos rendering and loggign messages to read data safely
	- std::vector: stores multiple entities in the world 
	- std::to_string: converts numbers into strings for log messages. 

The Log function creates structured debug messages that include: 
- the frame number 
- the log level
- a message describing what happened

Example: 
	[Frame 3] [INFO] Goblin took 20 damage. Health: 15 -> 0

Good debug output shoud show: 
	- what object changed
	- what value changed
	- the old value
	- the new value
	- when the change happened
	- whether the evetn is normal, suspicious, or serious

This lab models an important game development idea: 
hidden state changes should be made visible while debugging. 

Debug logs help diagnose issues such as: 
	- damage being applied twice
	- defeating entities still receiving actions
	- health or stamina reaching invalid values
	- state changes happening on the wrong frame
	- systems running when they shoud not

This connects to larger systems concepts such as: 
	- observability
	- event tracing
	- debugging tools
	- logging levels
	- state inspection 
	- profiling and diagnostics 

The main idea: 
	debug output should not just print data; it should explain meaningful state changes 
	so the behavior of the simulation can be understood frame by frame

*/
#include <iostream>
#include <string>	
#include <vector>	

enum class LogLevel
{
	Info,
	Warning,
	Error
};

struct Entity
{
	std::string name;
	int health;
	int maxHealth;
	int stamina;
	int maxStamina;
	bool alive;
};

std::string LogLevelToString(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Info:
		return "INFO";

	case LogLevel::Error:
		return "ERROR";

	case LogLevel::Warning:
		return "WARNING";

	default:
		return "UNKNOWN";
	}
}

void Log(LogLevel level, int frame, const std::string& message)
{
	std::cout << "[Frame " << frame << "] "
		<< "[" << LogLevelToString(level) << "] "
		<< message
		<< "\n";
}

int ClampInt(int value, int minValue, int maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

void ApplyDamage(Entity& entity, int amount, int frame)
{
	if (!entity.alive)
	{
		Log(LogLevel::Warning, frame, entity.name + " is already defeated. Damage ignored.");
		return;
	}

	int oldHealth = entity.health;
	entity.health -= amount;
	entity.health = ClampInt(entity.health, 0, entity.maxHealth);

	Log(LogLevel::Info,
		frame,
		entity.name + " took " + std::to_string(amount)
		+ " damage. Health: "
		+ std::to_string(oldHealth)
		+ " -> "
		+ std::to_string(entity.health)
	);

	if (entity.health == 0)
	{
		entity.alive = false;
		Log(LogLevel::Error, frame, entity.name + " was defeated.");
	}
}

void UseStamina(Entity& entity, int amount, int frame)
{
	int oldStamina = entity.stamina;

	entity.stamina -= amount;
	entity.stamina = ClampInt(entity.stamina, 0, entity.maxStamina);

	Log(
		LogLevel::Info,
		frame,
		entity.name + " used " + std::to_string(amount)
		+ " stamina. Stamina: "
		+ std::to_string(oldStamina)
		+ " -> "
		+ std::to_string(entity.stamina)
	);

	if (entity.stamina == 0)
	{
		Log(LogLevel::Warning, frame, entity.name + " is out of stamina.");
	}
}

void RegenerateStamina(Entity& entity, int amount, int frame)
{
	if (!entity.alive)
	{
		Log(LogLevel::Warning, frame, entity.name + " cannot regenerate stamina because they are defeated.");
		return;
	}

	int oldStamina = entity.stamina;

	entity.stamina += amount;
	entity.stamina = ClampInt(entity.stamina, 0, entity.maxStamina);
	Log(LogLevel::Info, frame, entity.name + " regenerated stamina for " + std::to_string(amount)
		+ ". Stamina: "
		+ std::to_string(oldStamina)
		+ " -> "
		+ std::to_string(entity.stamina)
	);

	if (entity.stamina == entity.maxStamina)
	{
		Log(LogLevel::Info, frame, entity.name + " stamina is full.");
	}
}

void Heal(Entity& entity, int amount, int frame)
{
	if (!entity.alive)
	{
		Log(LogLevel::Warning, frame, entity.name + " cannot be healed because they are defeated.");
		return;
	}

	int oldHealth = entity.health;

	entity.health += amount;
	entity.health = ClampInt(entity.health, 0, entity.maxHealth);

	Log(LogLevel::Info, frame, entity.name + " healed for " + std::to_string(amount)
		+ ". Health: "
		+ std::to_string(oldHealth)
		+ " -> "
		+ std::to_string(entity.health)
	);
}

void RenderEntity(const Entity& entity)
{
	std::cout << entity.name
		<< " | health: " << entity.health << "/" << entity.maxHealth
		<< " | stamina: " << entity.stamina << "/" << entity.maxStamina
		<< " | alive: " << (entity.alive ? "yes" : "no")
		<< "\n";
}

void RenderWorld(const std::vector<Entity>& entities)
{
	std::cout << "World State:\n";

	for (const Entity& entity : entities)
	{
		RenderEntity(entity);
	}
	std::cout << "----------------------\n";
}

int main()
{
	std::vector<Entity> entities
	{
		{"Player", 100, 100, 50, 100, true},
		{"Goblin", 30, 30, 10, 30, true}
	};

	for (int frame = 1; frame <= 5; ++frame)
	{
		Log(LogLevel::Info, frame, "Frame started.");

		if (frame == 1)
		{
			UseStamina(entities[0], 20, frame);
		}
		if (frame == 2)
		{
			ApplyDamage(entities[1], 15, frame);
		}
		if (frame == 3)
		{
			ApplyDamage(entities[1], 20, frame);
		}
		if (frame == 4)
		{
			ApplyDamage(entities[1], 10, frame);
			Heal(entities[1], 10, frame);
		}
		if (frame == 5)
		{
			Heal(entities[0], 25, frame);
			RegenerateStamina(entities[0], 80, frame);
		}

		RenderWorld(entities);
	}

	return 0;
}

