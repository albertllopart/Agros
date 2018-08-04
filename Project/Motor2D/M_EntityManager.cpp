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
Entity* EntityManager::CreateInfantry(iPoint position, entity_army army)
{
	Infantry* infantry = new Infantry();
	infantry->position = position;
	infantry->entity_army = army;
	infantry->Awake(App->GetConfigNode("entities"));
	infantry->Start();

	entities.add(infantry);
	units.add(infantry);

	return infantry;
}

//BUILDINGS
Entity* EntityManager::CreateBase(iPoint position, entity_army army)
{
	Base* base = new Base();
	base->entity_army = army;
	base->Awake(App->GetConfigNode("entities"));
	base->Start();
	base->position = position;

	entities.add(base);
	buildings.add(base);

	return base;
}

Entity* EntityManager::CreateFactory(iPoint position, entity_army army)
{
	Factory* factory = new Factory();
	factory->entity_army = army;
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
					App->player->state = NAVIGATING;
					break;
				}

				case ATTACK:
				{
					App->gui->DisableMenu(button->mtype);
					App->player->state = TARGETING;
					App->input->state = PLAYER_INPUT;

					Unit* unit = (Unit*)App->player->selected_unit;
					unit->targeted_unit = unit->targets.start->data;
					
					App->player->active = true;
					App->input->keyboard[SDL_SCANCODE_O] = KEY_UP;
					break;
				}

				case BUY_CANI_INFANTRY:
				{
					CreateInfantry(App->player->selected_unit->position, CANI);
					App->player->selected_unit->OnRelease();
					App->gui->DisableMenu(button->mtype);
					App->input->state = PLAYER_INPUT;
					break;
				}

				case BUY_HIPSTER_INFANTRY:
				{
					CreateInfantry(App->player->selected_unit->position, HIPSTER);
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
					
					if (App->player->turn == CANI)
						App->player->turn = HIPSTER;
					else if (App->player->turn == HIPSTER)
						App->player->turn = CANI;

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