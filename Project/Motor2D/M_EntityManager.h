#ifndef __M_ENTITYMANAGER_H__
#define __M_ENTITYMANAGER_H__

#include "Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "Entity.h"
#include "Cani.h"

class EntityManager : public Module
{
public:

	EntityManager();
	virtual ~EntityManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	Entity* CreateCani(iPoint position);

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

public:

	p2List<Entity*>			entities;

};

#endif