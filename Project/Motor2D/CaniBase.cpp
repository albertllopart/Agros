#include "CaniBase.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"

Canibase::Canibase() : Building()
{
	name.create("Canibase");
}

Canibase::~Canibase()
{}

bool Canibase::Awake(pugi::xml_node& config)
{
	pugi::xml_node node = config.child("buildings").child("canibase").child("animation");

	sprite.x = node.child("idle").child("frame_1").attribute("x").as_int();
	sprite.y = node.child("idle").child("frame_1").attribute("y").as_int();
	sprite.w = node.child("idle").child("frame_1").attribute("w").as_int();
	sprite.h = node.child("idle").child("frame_1").attribute("h").as_int();

	return true;
}

bool Canibase::Start()
{
	graphic = App->tex->Load("textures/buildings_cani.png");

	state = IDLE;
	entity_type = BUILDING;
	building_type = CANIBASE;

	return true;
}

bool Canibase::Update(float dt)
{
	Draw();

	return true;
}

bool Canibase::CleanUp()
{
	return true;
}

void Canibase::Draw()
{
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(graphic, world_position.x, world_position.y + OFFSET, &sprite);
}
