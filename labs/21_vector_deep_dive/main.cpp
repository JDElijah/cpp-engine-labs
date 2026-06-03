#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class Entity
{
private: 
	int id; 
	std::string name;
	int health; 
	bool alive; 

public: 
	Entity(int entityId, const std::string& entityName, int entityHealth)
		: id(entityId), name(entityName), health(entityHealth), alive(true)
	{
		std::cout << "Entity created: " << name << " [id: " << id << "]\n";
	}

	Entity(const Entity& other)
		: id(other.id), name(other.name), health(other.health), alive(other.alive)
	{
		std::cout << "Entity copied: " << name << " [id: " << id << "]\n";
	}

	Entity(Entity&& other) noexcept
		: id(other.id), name(other.name), health(other.health), alive(other.alive)
	{
		std::cout << "Entity moved: " << name << " [id: " << id << "]\n";
		other.id = -1; 
		other.health = 0;
		other.alive = false;
	}

	Entity& operator=(const Entity& other)
	{
		if (this != &other)
		{
			id = other.id; 
			name = other.name; 
			health = other.health; 
			alive = other.alive; 

			std::cout << "Entity copy-assigned: " << name << " [id: " << id << "]\n";
		}

		return *this; 
	}

	Entity& operator=(Entity&& other) noexcept
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

			std::cout << "Entity move-assigned: " << name << " [id: " << id << "]\n";
		}
		
		return *this; 
	}

	~Entity()
	{
		std::cout << "Entity destroyed: " << name << " [id: " << id << "]\n";
	}

};