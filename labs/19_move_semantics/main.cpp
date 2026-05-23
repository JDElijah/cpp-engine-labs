/*
Lab 19: Move Semantics

This exercise demonstrates the difference between copying an object and moving the object.

Core C++ concepts:
	- copy constructor: creates a new object by duplicating an existing object.
	- move constructor: creates a new object by transferring state from another object.
	- copy assignment: replaces an existing object by copying another object.
	- move assignment: replaces an existing object by moving from another object.
	- std::move: marks an object as allowed to be moved from.
	- moved-from state: the original still exists, but its meaningful data may have been transferred.
	- std::vector relocation: vector may move existing elements internally when they grow and need more capacity

Copying means duplication

Example:
	inventory.AddItemCopy(worldRifle);

The Rifle in the world is copied into the inventory. After the copy, there are two separate
Rifle objects: the original world Rifle, and the inventory Rifle.

Moving means transfer.

Example:
	inventory.AddItemMove(std::move(worldShotgun));

The Shotgun's state is moved into the inventory. The original worldShogun object still exists as a C++
object, but it should no longer be treated as the active gameplay Shotgun.

This models an important game development idea:
	Some objects should not be duplicated when they move between systems.
	Instead, their ownership/state should transfer from one plce to another.

Examples:
	- a loot item moves from the world into a player inventory.
	- an inventory item is dropped back into the world.
	- a projectile or resource changes ownership between sytems
	- a unique item keeps its ammo, durability, attachments, and condition.

This lab also shows that std::vector may move objects interally. When a vector
needs more space, it can allocate new memory and move existing elements into that new
memory. That is why an item like Rifle may be moved even if std::move was not called
on worldRifle directly.

This connects to larger systems concepts such as:
	- ownership transfer
	- persistent item state
	- inventory systems
	- world loot systems
	- avoiding item duplication bugs
	- resource-safe transfer
	- move-only types lile std::unique_ptr
	- container memory management

The main idea:
	Copying creates another object, while moving transfers an object's state or resources
	to a new owner. Use copying when duplication is intended, and use moving when ownership
	shoud change without creating a second meaningful object.
*/
#include <iostream>			// std::cout
#include <string>			// std::string
#include <vector>			// std::vector
#include <utility>			// std::move

class Item
{
private:
	std::string name;
	int durability;
	int ammo;

public:
	Item(const std::string& itemName, int itemDurability, int itemAmmo)
		: name(itemName), durability(itemDurability), ammo(itemAmmo)
	{
		std::cout << "Item created: " << name << "\n";
	}

	Item(const Item& other)
		: name(other.name), durability(other.durability), ammo(other.ammo)
	{
		std::cout << "Item copied: " << name << "\n";
	}

	Item(Item&& other) noexcept
		: name(std::move(other.name)),
		durability(other.durability),
		ammo(other.ammo)
	{
		std::cout << "Item moved: " << name << "\n";

		other.durability = 0;
		other.ammo = 0;
	}

	Item& operator=(const Item& other)
	{
		if (this != &other)
		{
			name = other.name;
			durability = other.durability;
			ammo = other.ammo;

			std::cout << "Item copy-assigned: " << name << "\n";
		}

		return *this;
	}

	Item& operator=(Item&& other) noexcept
	{
		if (this != &other)
		{
			name = std::move(other.name);
			durability = other.durability;
			ammo = other.ammo;

			other.durability = 0;
			other.ammo = 0;

			std::cout << "Item move-assigned: " << name << "\n";
		}

		return *this;
	}

	~Item()
	{
		std::cout << "Item destroyed: " << name << "\n";
	}

	void Use()
	{
		if (ammo > 0)
		{
			ammo--;
		}

		durability -= 5;

		if (durability < 0)
		{
			durability = 0;
		}
	}

	void PrintStatus() const
	{
		std::cout << name
			<< " | durability: " << durability
			<< " | ammo: " << ammo
			<< "\n";
	}
};

class Inventory
{
private:
	std::vector<Item> items;

public:
	void AddItemCopy(const Item& item)
	{
		std::cout << "Adding item by copy\n";
		items.push_back(item);
	}

	void AddItemMove(Item&& item)
	{
		std::cout << "Adding item by move\n";
		items.push_back(std::move(item));
	}

	Item& GetItem(int index)
	{
		return items[index];
	}

	Item DropItem(int index)
	{
		Item droppedItem = std::move(items[index]);
		items.erase(items.begin() + index);
		return droppedItem;
	}

	void PrintInventory() const
	{
		std::cout << "Inventory:\n";

		if (items.empty())
		{
			std::cout << " Empty\n";
			return;
		}

		for (const Item& item : items)
		{
			std::cout << " ";
			item.PrintStatus();
		}
	}
};

int main()
{
	std::cout << "Program started\n";

	Item worldRifle("Rifle", 80, 12);

	std::cout << "World item before pickup:\n";
	worldRifle.PrintStatus();

	std::cout << "----------------------\n";

	Inventory playerInventory;

	std::cout << "Copy pickup example:\n";
	playerInventory.AddItemCopy(worldRifle);

	std::cout << "World item after copy pickup:\n";
	worldRifle.PrintStatus();

	std::cout << "Player inventory after copy pickup:\n";
	playerInventory.PrintInventory();

	std::cout << "----------------------\n";

	Item worldShotgun("Shotgun", 60, 6);

	std::cout << "World item before move pickup:\n";
	worldShotgun.PrintStatus();

	playerInventory.AddItemMove(std::move(worldShotgun));

	std::cout << "World item after move pickup:\n";
	worldShotgun.PrintStatus();

	std::cout << "Player inventory after move pickup:\n";
	playerInventory.PrintInventory();

	std::cout << "----------------------\n";

	std::cout << "Using moved shotgun inside inventory:\n";
	Item& inventoryShotgun = playerInventory.GetItem(1);
	inventoryShotgun.Use();
	inventoryShotgun.PrintStatus();

	std::cout << "Player inventory after using shotgun:\n";
	playerInventory.PrintInventory();

	std::cout << "----------------------\n";

	std::cout << "Dropping shotgun back into the world:\n";
	Item droppedShotgun = playerInventory.DropItem(1);

	std::cout << "Dropped shotgun state:\n";
	droppedShotgun.PrintStatus();

	std::cout << "Player inventory after dropping shotgun:\n";
	playerInventory.PrintInventory();

	std::cout << "Program ended\n";

	return 0;


}