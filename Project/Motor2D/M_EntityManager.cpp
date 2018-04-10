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

Entity* EntityManager::CreateCani(iPoint position)
{
	Cani* cani = new Cani();
	cani->Awake(App->GetConfigNode("entities"));
	cani->Start();
	cani->position = position;

	entities.add(cani);

	return cani;
}

bool EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}