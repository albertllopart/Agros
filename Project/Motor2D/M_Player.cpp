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
#include "M_EntityManager.h"

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
	navigating.PushBack({ node.child("idle").child("frame_1").attribute("x").as_int(), node.child("idle").child("frame_1").attribute("y").as_int(), node.child("idle").child("frame_1").attribute("w").as_int(), node.child("idle").child("frame_1").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_1").attribute("x").as_int(), node.child("idle").child("frame_1").attribute("y").as_int(), node.child("idle").child("frame_1").attribute("w").as_int(), node.child("idle").child("frame_1").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_2").attribute("x").as_int(), node.child("idle").child("frame_2").attribute("y").as_int(), node.child("idle").child("frame_2").attribute("w").as_int(), node.child("idle").child("frame_2").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_2").attribute("x").as_int(), node.child("idle").child("frame_2").attribute("y").as_int(), node.child("idle").child("frame_2").attribute("w").as_int(), node.child("idle").child("frame_2").attribute("h").as_int() });

	navigating.speed = node.child("idle").attribute("speed").as_float();

	return ret;
}

bool Player::Start()
{
	LOG("Starting Player");
	bool ret = true;

	graphic = App->tex->Load("textures/player.png");

	state = NAVIGATING;

	return ret;
}

bool Player::Update(float dt)
{
	LOG("Updating Player");

	Input(dt);

	if (state == UNIT_SELECTED)
		App->map->DrawBFS();

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
	//moving
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		position.y -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		position.y += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		position.x += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		position.x -= 1;
	}

	//controlling units
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && selected_unit == NULL)
	{
		for (p2List_item<Entity*>* item = App->entities->entities.start; item; item = item->next)
		{
			if (item->data->position == position)
			{
				selected_unit = item->data;
				selected_unit->OnSelection();
				state = UNIT_SELECTED;

				//BFS
				App->map->ResetBFS(position);
				App->map->PropagateBFS();
				break;
			}
		}
		if (selected_unit == NULL)
		{
			//obrir el menú
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (selected_unit != NULL)
		{
			selected_unit->OnRelease();
			selected_unit = nullptr;
			state = NAVIGATING;
		}
	}
}

void Player::Draw()
{
	switch (state)
	{
		case NAVIGATING:
		{
			current_animation = &navigating;
			break;
		}
	}

	SDL_Rect r = current_animation->GetCurrentFrame();
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(graphic, OFFSET + world_position.x - 1, OFFSET + world_position.y, &r);
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