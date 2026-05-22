/*
Lab 18: Copy vs Reference Semantics

This exercise demonstrates the difference between copying an objecty and
working with the original object through a reference. 

Core C++ concepts practiced: 
	- pass by value: create a copy of an object. 
	- pass by reference: gives access to the original object.
	- const reference: reads the original object without copying or modifying it 
	- copy constructor: runs when a new object is created from an existing object
	- std::vector: stores copied objects inside a container. 
	- object identity: understanding whether code affects a copy or the real object. 

Pass by value creates a separate object. 

Example: 
	void UseItemByValue(Item item); 

When sword is passed into this function, C++ copies it. The function modifies the copy, 
so the original sword is unchanged. 

Passing by reference modies the original object. 

Example: 
	void UsItemByReference(Item& item); 

When sword is passed into this function, no copy is made. The function modifies the real sword

Passing by const reference is best for read-only access. 

Example: 
	void PrintItemByConstReference(const Item& item); 

This avoids copying while preventing accidental modication. 

The Player inventory challenge reinforces the same idea: 
	- AddItem(const Item& item) copies an item into the inventory
	- GetItem(int index) returns Item&, a reference to the real itme inside the vector
	- using that retured reference modifies the inventory item itself. 

This models an important game development idea: 
	some data can be safely copied, but game objects with identity often need to  be 
	modified through references. 

Examples: 
	- copying an item may create a separate inventory item.
	- damaging an enemy by value would damage only a copy. 
	- updating position be reference changes the real entity
	- rendering by const reference reads state safely without changing it. 

This connects to larger systems concepts as well: 
	- value vs identity
	- mutation vs observation 
	- performance cost of copying 
	- API design
	- inventory systems
	- entity updates
	- debugging state changes 
	- avoidng accidental copies in game logic

The main idea: 
	Know whether a function is working with a copy or the original object, because 
	that determines whether state changes persist.

*/

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>	

class Item
{
private:
	std::string name;
	int durability;

public:
	Item(const std::string& itemName, int itemDurability)
		: name(itemName), durability(itemDurability)
	{
		std::cout << "Item created: " << name << "\n";
	}

	Item(const Item& other)
		: name(other.name), durability(other.durability)
	{
		std::cout << "Item copied: " << name << "\n";
	}

	~Item()
	{
		std::cout << "Item destroyed: " << name << "\n";
	}

	const std::string& GetName() const 
	{
		return name; 
	}

	void Use()
	{
		durability -= 10;

		if (durability < 0)
		{
			durability = 0;
		}
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " | durability: " << durability
			<< "\n";
	}
};

class Player 
{
private: 
	std::string name; 
	std::vector<Item> inventory; 

public: 
	Player(const std::string& playerName)
		: name(playerName)
	{
		std::cout << "Player created: " << name << "\n";
	}

	void AddItem(const Item& item)
	{
		inventory.push_back(item); 
	}

	void PrintInventory() const
	{
		if (inventory.empty())
		{
			std::cout << "Player: " << name << "'s inventory is empty.\n";
			return;
		}
		
		for (const Item& item : inventory)
		{
			item.PrintStatus(); 
		}
	}

	Item& GetItem(int index)
	{
		if (index < 0 || index >= inventory.size())
		{
			throw std::out_of_range("Invalid inventory index");
		}
		return inventory[index];
	}
};

void UseItemByValue(Item item)
{
	std::cout << "Entering UseItemByValue\n";

	item.Use();
	item.PrintStatus();

	std::cout << "Leaving UseItemByValue\n";
}

void UseItemByReference(Item& item)
{
	std::cout << "Entering UseItemByReference\n";

	item.Use();
	item.PrintStatus();

	std::cout << "Leaving UseItemByReference\n";
}

void PrintItemByConstReference(const Item& item)
{
	std::cout << "Printing item by const reference\n";
	item.PrintStatus();
}


int main()
{
	std::cout << "Program started\n";

	Item sword("Iron Sword", 100);

	std::cout << "Initial sword:\n";
	sword.PrintStatus();

	std::cout << "----------------------\n";

	std::cout << "Using item by value:\n";
	UseItemByValue(sword);

	std::cout << "Sword after UseItemByValue:\n";
	sword.PrintStatus();

	std::cout << "----------------------\n";

	std::cout << "Using item by reference:\n";
	UseItemByReference(sword);

	std::cout << "Sword after UseItemByReference:\n";
	sword.PrintStatus();

	std::cout << "----------------------\n";

	std::cout << "Printing item by const reference:\n";
	PrintItemByConstReference(sword);

	std::cout << "----------------------\n";

	Player playerJosh("Josh"); 

	playerJosh.AddItem(sword);

	std::cout << "Player inventory after adding sword:\n";
	playerJosh.PrintInventory();

	std::cout << "Using inventory item by reference:\n";
	Item& inventorySword = playerJosh.GetItem(0);
	UseItemByReference(inventorySword); 
	

	std::cout << "Player inventory after using inventory sword:\n";
	playerJosh.PrintInventory(); 

	std::cout << "Program ended\n";

	return 0;
}