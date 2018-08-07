#pragma once
#include "p2Point.h"
#include "Entity.h"
#include "Building.h"
#include "Animation.h"

#define OFFSET -2
#define SIZE 16

struct SDL_Texture;
struct SDL_Rect;

class Factory : public Building
{
public:

	Factory();
	virtual~Factory();

	// Called before render is available
	bool				 Awake(pugi::xml_node& config);

	// Called before the first frame
	bool				Start();

	// Called each loop iteration
	bool				Update(float dt);

	// Called before quitting
	bool				CleanUp();

	void				Draw();

	bool				OnSelection();

	bool				OnRelease();

	void				ChangeArmy(int new_army);

public:

};