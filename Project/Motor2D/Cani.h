#pragma once
#include "p2Point.h"
#include "Entity.h"
#include "Animation.h"

#define OFFSET -2
#define SIZE 16

struct SDL_Texture;

class Cani : public Entity
{
public:

	Cani();
	virtual~Cani();

	// Called before render is available
	bool				 Awake(pugi::xml_node& config);

	// Called before the first frame
	bool				Start();

	// Called each loop iteration
	bool				Update(float dt);

	// Called before quitting
	bool				CleanUp();

	void				Draw();

	iPoint				GetPosition() const;

	//interaction with player
	bool				OnSelection();
	bool				OnRelease();

public:

	Animation			idle_right;
	Animation			walk_right;
	Animation			walk_up;
	Animation*			current_animation;

};