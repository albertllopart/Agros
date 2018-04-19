#pragma once
#include "p2Point.h"
#include "Entity.h"
#include "Animation.h"

struct SDL_Texture;
struct SDL_Rect;

enum building_type
{
	FACTORY,
	CANIBASE
};

class Building : public Entity
{
public:

	Building() : Entity()
	{}
	virtual~Building()
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

	virtual bool				OnSelection()
	{
		return true;
	}

	virtual bool				OnRelease()
	{
		return true;
	}

public:

	building_type		building_type;

	SDL_Rect			sprite;

};