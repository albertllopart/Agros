#ifndef __M_ENTITYMANAGER_H__
#define __M_ENTITYMANAGER_H__

#include "Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "Entity.h"
#include "Infantry.h"
#include "Base.h"
#include "Factory.h"

class EntityManager : public Module
{
public:

	EntityManager();
	virtual ~EntityManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	//UNITS
	Entity* CreateInfantry(iPoint position, entity_army army);

	//BUILDINGS
	Entity* CreateBase(iPoint position, entity_army army);
	Entity* CreateFactory(iPoint position, entity_army army);

	//GUI
	void GuiTrigger(GuiElement* element);

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

public:

	p2List<Entity*>			entities;
	p2List<Unit*>			units;
	p2List<Building*>		buildings;

};

#endif