#include "Factory.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "M_Input.h"
#include "M_Player.h"

Factory::Factory() : Building()
{
	name.create("factory");
}

Factory::~Factory()
{}

bool Factory::Awake(pugi::xml_node& config)
{
	pugi::xml_node node = config.child("buildings").child("factory").child("animation");

	if (entity_army == CANI)
	{
		sprite.x = node.child("cani").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("cani").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("cani").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("cani").child("idle").child("frame_1").attribute("h").as_int();
	}

	else if (entity_army == HIPSTER)
	{
		sprite.x = node.child("hipster").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("hipster").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("hipster").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("hipster").child("idle").child("frame_1").attribute("h").as_int();
	}

	else if (entity_army == NEUTRAL)
	{
		sprite.x = node.child("neutral").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("neutral").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("neutral").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("neutral").child("idle").child("frame_1").attribute("h").as_int();
	}

	return true;
}

bool Factory::Start()
{
	graphic = App->tex->Load("textures/buildings_cani.png");

	state = IDLE;
	entity_type = BUILDING;
	building_type = FACTORY;

	return true;
}

bool Factory::Update(float dt)
{
	Draw();

	return true;
}

bool Factory::CleanUp()
{
	return true;
}

void Factory::Draw()
{
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(graphic, world_position.x, world_position.y + OFFSET, &sprite);
}

bool Factory::OnSelection()
{
	state = SELECTED;

	App->player->active = false;
	App->input->state = UI_INPUT;

	if (entity_army == CANI)
		App->gui->ActivateMenu(CANI_SHOP_MENU);
	else if (entity_army == HIPSTER)
		App->gui->ActivateMenu(HIPSTER_SHOP_MENU);

	return true;
}

bool Factory::OnRelease()
{
	state = IDLE;

	App->player->selected_unit = nullptr;
	App->player->active = true;
	App->player->state = NAVIGATING;

	return true;
}
