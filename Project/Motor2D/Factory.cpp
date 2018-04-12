#include "Factory.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"

Factory::Factory() : Building()
{
	name.create("factory");
}

Factory::~Factory()
{}

bool Factory::Awake(pugi::xml_node& config)
{
	pugi::xml_node node = config.child("buildings").child("factory").child("animation");

	sprite.x = node.child("idle").child("frame_1").attribute("x").as_int();
	sprite.y = node.child("idle").child("frame_1").attribute("y").as_int();
	sprite.w = node.child("idle").child("frame_1").attribute("w").as_int();
	sprite.h = node.child("idle").child("frame_1").attribute("h").as_int();

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
