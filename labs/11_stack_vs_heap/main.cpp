/*
Lab 11: Stack vs Heap 

This exercise demonstrates the difference between stack allocation and heap allocation in C++. 

Core C++ concepts practiced: 
	- stack allocation: objects created directly inside a scope.
	- heap allocation: objects created manually with new. 
	- constructors: code that run when an object is created. 
	- destructors: code that runs when an object is destroyed. 
	- pointers: variables that store memory addresses. 
	- delete: manually destroys a heap-allocated object. 
	- nullptr: represents a pointer that points to nothing. 
	- scope: controls automatic lifetime for stack objects. 

Stack objects are created and destroyed automatically. 
Example: 
	Enemy goblin("Stack Goblin", 30) ; 
When the function or scope ends, the destructor runs automatically. 

Heap objects are created manually and must be destroyed manually. 
Example: 
	Enemy* skeleton = new Enemy("Heap Skeleton, 40); 
	delete skeleton; 
	skeleton = nullptr; 

If a heap object is created with new but never deleted, the program leaks memory. 
It a pointer is used after delete, it becomes a dangerous dangling pointer. 

This lab models an important game development idea: 
game objects and resources have lifetimes. 

Examples: 
	- enemies spawn and die
	- bullets are created and destroyed
	- particles expire 
	- sounds start and finish
	- levels load and unload
	- textures and assets must be released when no longer needed. 

The main question is: 
who owns this object, and when should it be destroyed?

This connects to larger C++ and engine concepts such as: 
	- RAII
	- std::unique_ptr
	- std::shared_ptr
	- resource managers
	- object pools
	- memory leaks
	- dangling pointers 
	- ownership and lifetime design

The main ideaL 
stack objects clean themselves up automatically when scope ends, while manually allocated
heap objects must be explicitly cleaned up or managed by safer modern C++ tools. 

A useful rule of thumb

Use the stack when:

the object has a clear, short lifetime
the object belongs to the current function or scope
it should be destroyed automatically when the scope ends

Examples:

temporary vectors
damage calculation data
collision test results
local timers
small helper objects

Use the heap or a managed heap structure when:

the object must live beyond the current function
ownership must be transferred or stored
many systems need to access it over time
the object is part of the world/session/level

Examples:

spawned enemies
projectiles
UI screens
loaded resources
game states
large dynamic collections

The real question is not:

“Should I use stack or heap?”

The better question is:

“What is the lifetime of this object, and who is responsible for ending it?”

For the arrow:

Who owns it?
The GameWorld, ProjectileSystem, or ObjectPool.

When should it be destroyed?
When it hits something, expires, leaves bounds, or the level unloads.

For the aim direction:

Who owns it?
The attack function.

When should it be destroyed?
Immediately when the function ends.

That is the core idea.

Stack and heap are tools. Ownership and lifetime are the design decisions.
*/
#include <iostream>
#include <string>

struct Enemy // In C++, struct and class are the same tool with different default privacy. 
{
	std::string name; 
	int health; 

	Enemy(const std::string& enemyName, int enemyHealth) : name(enemyName), health(enemyHealth)
	{
		std::cout << "Constructed enemy: " << name << "\n"; 
	}

	~Enemy()
	{
		std::cout << "Destroyed enemy: " << name << "\n"; 
	}

	void Attack() const
	{
		std::cout << name << " attacks!\n";
	}
};

void StackEnemyExample()
{
	std::cout << "Entering StackEnemyExample\n"; 

	Enemy goblin("Stack Goblin", 30);
	goblin.Attack(); 

	std::cout << "Leaving StackEnemyExample\n";
}

void HeapEnemyExample()
{
	std::cout << "Entering HeapEnemyExample\n"; 
	Enemy* skeleton = new Enemy("Heap Skeleton", 40); 
	skeleton->Attack(); 

	delete skeleton; 
	skeleton = nullptr; 

	std::cout << "Leaving HeapEnemyExample\n"; 
}

void HeapLeakExample()
{
	std::cout << "Entering HeapLeakExample\n"; 

	Enemy* ghost = new Enemy("Leaky Ghost", 10); 
	ghost->Attack();

	/*
		On run, notice that "Destroyed enemy: Leaky Ghost" didn't appear. This demonstrates a memory leak. 
		fix the leak by adding: 
		
		delete ghost; 
		ghost = nullptr; 
	*/

	std::cout << "Leaving HeapLeakExample without delete.\n"; 
}

int main()
{
	std::cout << "Program started\n"; 

	StackEnemyExample(); 

	std::cout << "Back in main after stack example\n";

	HeapEnemyExample(); 
	
	std::cout << "Back in main after heap example\n"; 

	HeapLeakExample();

	std::cout << "Program ended\n"; 

	return 0; 
}

