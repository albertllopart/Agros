#include "M_EntityManager.h"
#include "Application.h"
#include "M_Input.h"
#include "p2Defs.h"

EntityManager::EntityManager() : Module()
{
	name.create("entities");
}

EntityManager::~EntityManager()
{}

bool EntityManager::Awake(pugi::xml_node& data)
{
	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	p2List_item<Entity*>* item = entities.start;
	while (item != NULL)
	{
		item->data->Update(dt);
		item = item->next;
	}
	return true;
}

bool EntityManager::CleanUp()
{
	return true;
}

//UNITS
Entity* EntityManager::CreateCani(iPoint position)
{
	Cani* cani = new Cani();
	cani->Awake(App->GetConfigNode("entities"));
	cani->Start();
	cani->position = position;

	entities.add(cani);

	return cani;
}

//BUILDINGS
Entity* EntityManager::CreateCanibase(iPoint position)
{
	Canibase* canibase = new Canibase();
	canibase->Awake(App->GetConfigNode("entities"));
	canibase->Start();
	canibase->position = position;

	entities.add(canibase);

	return canibase;
}

Entity* EntityManager::CreateFactory(iPoint position)
{
	Factory* factory = new Factory();
	factory->Awake(App->GetConfigNode("entities"));
	factory->Start();
	factory->position = position;

	entities.add(factory);

	return factory;
}

bool EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}