/*
Lab 15: RAII

This exercise demonstrates RAII: Resource Acquisition Is Initialization. 

RAII means tying a resource's lifetme to an object's lifetime. 

Core C++ concepts practiced: 
	- constructors: acquire or initialize a resource when an object is created. 
	- destructors: release or clean up a resource wehn an object is destroyed
	- scope: controls when stack objects are automatically destroyed
	- classes: hide resources state behind a safer interface
	- const member functions: allow read-only operations like Bind(), Play(), and GetFilePath()
	- automatic cleanup: avoids needing manual unload calls. 

The Texture and SoundEffect classes simulate resource-owning objects. 

Texture: 
	- loads a texture in its constructor
	- binds/uses the texture with Bind()
	- unloads the texture in its destructor

SoundEffect: 
	- loads a sound effect in its constructor
	- plays the sound with Play()
	- unloads the sound in its destructor

The RAII pattern is: 
	constructor acquires the resource. 
	object is used while it is alive. 
	destructor releases the resources automatically.

This lab models an important game development idea: 
resources such as textures, sounds, meshes, files, shaders, and GPU buiggers must have clear ownership and cleanup rules. 

For this lab, PlayAttackSound() creates a SoundEffect locally to clearly show scope-based lifetime. 
	enter function -> load sound -> play sound -> leave function -> unload sound

In real gameplay, reusable assets are usually loaded once by a longer-lived system, such as a resource manage, and played many times. But the RAII concept is the same: 
	The object that owns the resource is responsible for releasing it. 

This connects to larger C++ and engine concepts such as: 
	- std::vector
	- std::string
	- std::fstream
	- std::unique_ptr
	- std::lock_guard
	- resource managers
	- asset caching
	- ownership
	- lifetime safety
	- preventing leaks

The main idea: 
design resource-owning objects so cleanup happens automatically when the object goes out of scope. 
*/
#include <iostream>
#include <string>

class Texture
{
private:
	std::string filePath;
	bool loaded;

public: 
	Texture(const std::string& path) : filePath(path), loaded(false)
	{
		std::cout << "Loading texture: " << filePath << "\n"; 
		loaded = true; 
	}

	~Texture()
	{
		if (loaded)
		{
			std::cout << "Unloading texture: " << filePath << "\n";
			loaded = false; 
		}
	}

	void Bind() const
	{
		if (!loaded)
		{
			std::cout << "Cannot bind unloaded texture.\n"; 
			return; 
		}
		std::cout << "Binding texture: " << filePath << "\n"; 
	}

	const std::string& GetFilePath() const
	{
		return filePath; 
	}
};

class SoundEffect
{
private:
	std::string filePath;
	bool loaded;

public:
	SoundEffect(const std::string& path) : filePath(path), loaded(false)
	{
		std::cout << "Loading sound effect: " << filePath << "\n";
		loaded = true; 
	}

	~SoundEffect()
	{
		if (loaded)
		{
			std::cout << "Unloading sound effect: " << filePath << "\n";
			loaded = false;
		}
	}

	void Play() const
	{
		if (!loaded)
		{
			std::cout << "Cannot play unloaded sound effect.\n"; 
			return; 
		}
		std::cout << "Playing sound effect: " << filePath << "\n";
	}

	const std::string& GetFilePath() const
	{
		return filePath;
	}
};

void PlayAttackSound()
{
	SoundEffect attackSound("attack.wav");
	attackSound.Play(); 
}

void DrawPlayer()
{
	std::cout << "Entering DrawPlayer\n"; 

	Texture playerTexture("player.png");
	playerTexture.Bind(); 

	std::cout << "Drawing player with " << playerTexture.GetFilePath() << "\n"; 

	std::cout << "Leaving DrawPlayer\n"; 
}

void DrawEnemy()
{
	std::cout << "Entering DrawEnemy\n"; 

	Texture enemyTexture("enemy.png"); 
	enemyTexture.Bind(); 

	std::cout << "Drawing enemy with " << enemyTexture.GetFilePath() << "\n";

	std::cout << "Leaving DrawEnemy\n"; 
}

int main()
{
	std::cout << "Program started\n"; 

	DrawPlayer(); 

	std::cout << "Back in main\n";

	DrawEnemy(); 

	std::cout << "Back in main\n";

	PlayAttackSound(); 

	std::cout << "Program ended.\n"; 

	return 0;
}