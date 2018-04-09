#include "p2Defs.h"
#include "p2Log.h"
#include "M_Player.h"
#include "p2List.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Input.h"
#include "M_Map.h"
#include "M_Audio.h"
#include "M_Scene.h"

#include "Brofiler\Brofiler.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

Player::Player() : Module()
{
	name.create("player");
}

Player::~Player()
{}

bool Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player");
	bool ret = true;

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	pugi::xml_node node = config.child("animation");

	//Animations from config.tmx
	idle.PushBack({ node.child("idle").child("frame_1").attribute("x").as_int(), node.child("idle").child("frame_1").attribute("y").as_int(), node.child("idle").child("frame_1").attribute("w").as_int(), node.child("idle").child("frame_1").attribute("h").as_int() });
	idle.PushBack({ node.child("idle").child("frame_2").attribute("x").as_int(), node.child("idle").child("frame_2").attribute("y").as_int(), node.child("idle").child("frame_2").attribute("w").as_int(), node.child("idle").child("frame_2").attribute("h").as_int() });

	return ret;
}

bool Player::Start()
{
	LOG("Starting Player");
	bool ret = true;

	graphic = App->tex->Load("textures/player.png");

	state = IDLE;

	return ret;
}

bool Player::Update(float dt)
{
	LOG("Updating Player");

	Input(0.1);
	Draw();

	return true;
}

bool Player::PostUpdate()
{
	return true;
}

bool Player::CleanUp()
{
	LOG("Destroying player");

	bool ret = true;

	return ret;
}

void Player::Input(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		position.y -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		position.y += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		position.x += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		position.x -= 1;
	}
}

void Player::Draw()
{
	switch (state)
	{
		case IDLE:
		{
			current_animation = &idle;
			break;
		}
	}

	SDL_Rect r = current_animation->GetCurrentFrame();
	App->render->Blit(graphic, OFFSET + position.x * TILE_WIDTH, OFFSET + position.y * TILE_WIDTH, &r);
}

bool Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node node = data.append_child("position");

	node.append_attribute("x") = position.x;
	node.append_attribute("y") = position.y;

	return true;
}

bool Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}