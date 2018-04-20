#include "M_EntityManager.h"
#include "Application.h"
#include "M_Input.h"
#include "p2Defs.h"
#include "GuiElement.h"
#include "GuiButton.h"
#include "M_Gui.h"
#include "M_Player.h"
#include "M_Map.h"

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
	//PRINTING ORDER SHOULD BE: BUILDINGS -> BFS -> UNITS -> ARROW
	p2List_item<Building*>* building = buildings.start;
	while (building != NULL)
	{
		building->data->Update(dt);

		building = building->next;
	}

	if (App->player->selected_unit != NULL && App->player->selected_unit->state == SELECTED && App->player->selected_unit->entity_type == UNIT)
	{
		App->map->DrawBFS();
	}

	p2List_item<Unit*>* unit = units.start;
	while (unit != NULL)
	{
		if (unit->data != App->player->selected_unit)
			unit->data->Update(dt);

		unit = unit->next;
	}

	if (App->player->active == false && App->player->selected_unit != NULL)
	{
		App->player->selected_unit->Update(dt);
	}

	return true;
}

bool EntityManager::PostUpdate()
{
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
	cani->position = position;
	cani->Awake(App->GetConfigNode("entities"));
	cani->Start();

	entities.add(cani);
	units.add(cani);

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
	buildings.add(canibase);

	return canibase;
}

Entity* EntityManager::CreateFactory(iPoint position)
{
	Factory* factory = new Factory();
	factory->Awake(App->GetConfigNode("entities"));
	factory->Start();
	factory->position = position;

	entities.add(factory);
	buildings.add(factory);

	return factory;
}

void EntityManager::GuiTrigger(GuiElement* element)
{
	switch (element->etype)
	{
		case BUTTON:
		{
			GuiButton* button = (GuiButton*)element;

			switch (button->btype)
			{
				case WAIT:
				{
					App->player->selected_unit->OnWait();
					App->gui->DisableMenu(button->mtype);
					App->input->state = PLAYER_INPUT;
					break;
				}

				case BUY_CANI:
				{
					CreateCani(App->player->selected_unit->position);
					App->player->selected_unit->OnRelease();
					App->gui->DisableMenu(button->mtype);
					App->input->state = PLAYER_INPUT;
					break;
				}
				
				case END_TURN:
				{
					p2List_item<Entity*>* item = entities.start;

					while (item != NULL)
					{
						item->data->state = IDLE;
						item = item->next;
					}
					App->gui->DisableMenu(button->mtype);
					App->input->state = PLAYER_INPUT;
					App->player->active = true;
					break;
				}
			}
			break;
		}
	}
}

bool EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}