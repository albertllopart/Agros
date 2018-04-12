#pragma once
#include "Application.h"
#include "p2Point.h"

struct SDL_Texture;

enum entity_state
{
	IDLE,
	SELECTED,
	MOVING
};

enum entity_direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

enum entity_type
{
	CANI
};

class Entity
{
public:

	Entity() {};
	virtual ~Entity() {};

	// Called before render is available
	virtual bool Awake(pugi::xml_node& config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual iPoint	Getposition() const
	{
		return position;
	}
	virtual entity_direction GetDirection()const
	{
		return direction;
	}

	//interaction with player
	virtual bool OnSelection()
	{
		return true;
	}

	virtual bool OnRelease()
	{
		return true;
	}

	//virtual load/save function
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&)const
	{
		return true;
	}

public:

	p2SString					name;

	entity_type					type;
	entity_direction			direction;
	entity_state				state;

	iPoint						position;

	SDL_Texture*				graphic = nullptr;

};