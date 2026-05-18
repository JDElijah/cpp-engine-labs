/*
Lab 17:std::shared_ptr and std::weak_ptr 

This exercise demonstrates shared ownership and safe non-owning observation in modern C++. 

Core C++ Concepts practiced: 
	- std::shared_ptr: allows multiple owners to share the same the same heap object. 
	- std::make_shared: safely creates an object managed by shared_ptr
	- use_count(): shows how many shared_ptr owners currently share the object. 
	- std::weak_ptr: observes a shared object without owning it. 
	- lock(): safely attempts to access the object watched by a weak_ptr 
	- scope: controls when shared_ptr owners are destroyed
	- declaration order: a class must be declared before another class can use it. 

std::shared_ptr is useful when multiple objects genuinely shared responsibility for keeping a resource alive.

Examples from this lab: 
	- multiple Sprite objects share the same Texture
	- multiple Ability objects share the same SoundEffect

As long as at least one shared_ptr owns the resource, the resource stays alive. 
When the last shared_ptr owner goes out of scope, the resouce is destroyed. 

std::weak_ptr is useful when something needs to observe or track an object without keeping it alive.

Example from this Lab: 
	- TargetTracker watches an Enemy through weak_ptr

The tracker can check whether the enemy still exists by calling lock(). 
If lock() succeeds, the enemy is still alive and can be accessed safely.
If lock() fails, the enemy has already been destroyed.

This models important game development ownership patterns: 
	- shared assets like textures, sounds, materials, and animation data may be used by many objets at lonce. 
	- target trackers, UI selection systems, AI systems, and observers may need to reference objects without 
	owning them. 
	
The main ownership rules are: 
	- use unique_ptr when one system clearly owns an object.
	- use shared_ptr when multiple owners truly share lifetime. 
	- use weak_ptr when observing something owned elsewhere

This connects to larger engine concepts such a: 
	- asset sharing
	- resource lifetime
	- ownership graphs 
	- avoiding memory leaks
	- avoiding dangling pointers
	- preventing shared_ptr ownership cycles

The main idea: 
shared_ptr keeps shared resources alive while they are still owned, and weak_ptr lets systems safely observe 
shared objects without extending their lifetime.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>	

class Texture
{
private:
	std::string filePath;

public:
	Texture(const std::string& path)
		: filePath(path)
	{
		std::cout << "Loading texture: " << filePath << "\n";

	}

	~Texture()
	{
		std::cout << "Unloading texture: " << filePath << "\n";
	}

	void Bind() const
	{
		std::cout << "Binding texture: " << filePath << "\n";
	}

	const std::string& GetFilePath() const
	{
		return filePath;
	}
};

class Sprite
{
private:
	std::string name;
	std::shared_ptr<Texture> texture;

public:
	Sprite(const std::string& spriteName, std::shared_ptr<Texture> spriteTexture)
		: name(spriteName), texture(spriteTexture)
	{
		std::cout << "Create sprite: " << name << "\n";
	}

	~Sprite()
	{
		std::cout << "Destroyed sprite: " << name << "\n";
	}

	void Draw() const
	{
		std::cout << "Drawing sprite: " << name << "\n";
		texture->Bind();
	}

	void PrintTextureUseCount()	const
	{
		std::cout << name
			<< " texture used count: "
			<< texture.use_count()
			<< "\n";
	}
};

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
			std::cout << name << " is already defeated.\n";
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

	bool IsAlive() const
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

class SoundEffect
{
private:
	std::string filePath;

public:
	SoundEffect(const std::string& path)
		: filePath(path)
	{
		std::cout << "Loading sound: " << filePath << "\n";
	}

	~SoundEffect()
	{
		std::cout << "Unloading sound: " << filePath << "\n";
	}

	void Play() const
	{
		std::cout << "Playing sound: " << filePath << "\n";
	}

	const std::string& GetFilePath() const
	{
		return filePath;
	}

};

class Ability
{
private:
	std::string name;
	std::shared_ptr<SoundEffect> soundEffect;

public:
	Ability(const std::string& abilityname, std::shared_ptr<SoundEffect> abilitySoundEffect)
		: name(abilityname), soundEffect(abilitySoundEffect)
	{
		std::cout << name << " created.\n";
	}

	~Ability()
	{
		std::cout << name << " destroyed.\n";
	}

	void Use() const
	{
		std::cout << name << " used.\n";
		soundEffect->Play();
	}

	void PrintSoundEffectUseCount() const
	{
		std::cout << name
			<< " sound effect used count: "
			<< soundEffect.use_count()
			<< "\n";
	}

};

class TargetTracker
{
private:
	std::weak_ptr<Enemy> target;

public:
	void SetTarget(const std::shared_ptr<Enemy>& newTarget)
	{
		target = newTarget;
		std::cout << "Target assigned.\n";
	}

	void PrintTargetStatus() const
	{
		std::shared_ptr<Enemy> lockedTarget = target.lock();

		if (lockedTarget)
		{
			std::cout << "Tracked target status:\n";
			lockedTarget->PrintStatus();
		}
		else
		{
			std::cout << "Tracked target no longer exists.\n";
		}
	}
};

void SharedTextureExample()
{
	std::cout << "Entering SharedTextureExample\n";

	std::shared_ptr<Texture> sharedTexture = std::make_shared<Texture>("enemy.png");

	{
		Sprite goblinSprite("Goblin Sprite", sharedTexture);
		Sprite orcSprite("Orc Sprite", sharedTexture);

		goblinSprite.PrintTextureUseCount();
		orcSprite.PrintTextureUseCount();

		std::cout << "Leaving inner sprite scope.\n";
	}

	std::cout << "Back in SharedTextureExample\n";
	std::cout << "Texture use count after sprites destroyed: "
		<< sharedTexture.use_count()
		<< "\n";

	std::cout << "Leaving SharedTextureExample\n";
}

void WeakTargetExample()
{
	std::cout << "Entering WeakTargetExample\n";

	TargetTracker tracker;

	{
		std::shared_ptr<Enemy> goblin = std::make_shared<Enemy>("Goblin", 40);

		tracker.SetTarget(goblin);
		tracker.PrintTargetStatus();

		goblin->TakeDamage(15);
		tracker.PrintTargetStatus();

		std::cout << "leaving enemy scope\n";
	}

	tracker.PrintTargetStatus();

	std::cout << "Leaving WeakTargetExample\n";
}

void SharedSoundEffectExample()
{
	std::cout << "Entering SharedSoundEffectExample\n";

	std::shared_ptr<SoundEffect> impactSound = std::make_shared<SoundEffect>("impact.wav");

	{
		Ability fireball("Fireball", impactSound);
		Ability iceBolt("Ice Bolt", impactSound);

		fireball.Use();
		iceBolt.Use();

		fireball.PrintSoundEffectUseCount();
		iceBolt.PrintSoundEffectUseCount();

		std::cout << "Leaving inner ability scope.\n";
	}

	std::cout << "Back in SharedSoundEffectExample\n";
	std::cout << "SoundEffect use count after abilities destroyed: "
		<< impactSound.use_count()
		<< "\n";

	std::cout << "Leaving SharedSoundEffectExample.\n";
}

int main()
{
	std::cout << "Program started\n";

	SharedTextureExample();

	std::cout << "----------------------\n";

	WeakTargetExample();

	std::cout << "----------------------\n";

	SharedSoundEffectExample();

	std::cout << "----------------------\n";

	std::cout << "Program ended.\n";

	return 0;
}

