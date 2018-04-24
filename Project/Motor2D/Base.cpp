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
	pugi::xml_node node = config.child("buildings").child("Base").child("animation");

	sprite.x = node.child("idle").child("frame_1").attribute("x").as_int();
	sprite.y = node.child("idle").child("frame_1").attribute("y").as_int();
	sprite.w = node.child("idle").child("frame_1").attribute("w").as_int();
	sprite.h = node.child("idle").child("frame_1").attribute("h").as_int();

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
	App->render->Blit(graphic, world_position.x, world_position.y + OFFSET, &sprite);
}


