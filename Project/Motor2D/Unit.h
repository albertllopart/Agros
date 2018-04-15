#pragma once
#include "p2Point.h"
#include "Entity.h"
#include "Animation.h"

#define OFFSET -2
#define SIZE 16

struct SDL_Texture;

enum unit_direction
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

enum unit_type
{
	CANI
};

class Unit : public Entity
{
public:

	Unit() : Entity()
	{}
	virtual~Unit()
	{}

	// Called before render is available
	virtual bool				 Awake(pugi::xml_node& config)
	{

		return true;
	}

	// Called before the first frame
	virtual bool				Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool				Update(float dt)
	{
		return true;
	}

	// Called before quitting
	virtual bool				CleanUp()
	{
		return true;
	}

	virtual void				Draw()
	{}

	//interaction with player
	virtual bool						OnSelection()
	{
		return true;
	}

	virtual bool						OnRelease()
	{
		return true;
	}

	//pathfinding
	virtual void						GetPath(iPoint goal)
	{}

	virtual void						Move(iPoint goal)
	{}

public:

	unit_direction		direction;
	unit_type			unit_type;
	uint				move_range;

	Animation			idle_right;
	Animation			walk_right;
	Animation			walk_up;
	Animation*			current_animation;

	//pathfinding
	p2List<iPoint>		path;
};