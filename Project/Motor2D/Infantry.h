#pragma once
#include "p2Point.h"
#include "Entity.h"
#include "Unit.h"
#include "Animation.h"

#define OFFSET -2
#define SIZE 16

struct SDL_Texture;

class Infantry : public Unit
{
public:

	Infantry();
	virtual~Infantry();

	// Called before render is available
	bool						 Awake(pugi::xml_node& config);

	// Called before the first frame
	bool						Start();

	// Called each loop iteration
	bool						Update(float dt);

	// Called before quitting
	bool						CleanUp();

	void						Draw();

	iPoint						GetPosition() const;

	//interaction with player
	bool						OnSelection();
	bool						OnRelease();
	bool						OnWait();
	bool						OnAttack();
	bool						OnCapture();
	bool						OnDying();

	//pathfinding
	void						GetPath(iPoint goal);
	void						Move(float dt);

	void						SearchTargets();
	void						ClearTargets();

	void						CancelAction();

public:

};