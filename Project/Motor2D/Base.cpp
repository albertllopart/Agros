#include "Base.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "M_Input.h"

Base::Base() : Building()
{
	name.create("base");
}

Base::~Base()
{}

bool Base::Awake(pugi::xml_node& config)
{
	offset.x = config.child("buildings").child("base").child("offset").attribute("x").as_int();
	offset.y = config.child("buildings").child("base").child("offset").attribute("y").as_int();

	pugi::xml_node node = config.child("buildings").child("base").child("animation");

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

	return true;
}

bool Base::Start()
{
	graphic = App->tex->Load("textures/buildings_cani.png");

	state = IDLE;
	entity_type = BUILDING;
	building_type = BASE;

	return true;
}

bool Base::Update(float dt)
{
	Draw();

	return true;
}

bool Base::CleanUp()
{
	return true;
}

void Base::Draw()
{
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(graphic, world_position.x, world_position.y + offset.y, &sprite);
}

void Base::ChangeArmy(int new_army)
{
	pugi::xml_node node = App->GetConfigNode("entities");
	node = node.child("buildings").child("base").child("animation");

	switch (new_army)
	{
	case 0:
	{
		entity_army = CANI;

		sprite.x = node.child("cani").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("cani").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("cani").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("cani").child("idle").child("frame_1").attribute("h").as_int();
		break;
	}

	case 1:
	{
		entity_army = HIPSTER;

		sprite.x = node.child("hipster").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("hipster").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("hipster").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("hipster").child("idle").child("frame_1").attribute("h").as_int();
		break;
	}

	case 2:
	{
		entity_army = NEUTRAL;

		sprite.x = node.child("neutral").child("idle").child("frame_1").attribute("x").as_int();
		sprite.y = node.child("neutral").child("idle").child("frame_1").attribute("y").as_int();
		sprite.w = node.child("neutral").child("idle").child("frame_1").attribute("w").as_int();
		sprite.h = node.child("neutral").child("idle").child("frame_1").attribute("h").as_int();
		break;
	}
	}
}


