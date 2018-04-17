#include "p2Defs.h"
#include "p2Log.h"
#include "M_Player.h"
#include "p2List.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Input.h"
#include "M_Map.h"
#include "M_Window.h"
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
	offset.x = config.child("offset").attribute("x").as_int();
	offset.y = config.child("offset").attribute("y").as_int();

	pugi::xml_node node = config.child("animation");

	//Animations from config.tmx
	navigating.PushBack({ node.child("idle").child("frame_1").attribute("x").as_int(), node.child("idle").child("frame_1").attribute("y").as_int(), node.child("idle").child("frame_1").attribute("w").as_int(), node.child("idle").child("frame_1").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_1").attribute("x").as_int(), node.child("idle").child("frame_1").attribute("y").as_int(), node.child("idle").child("frame_1").attribute("w").as_int(), node.child("idle").child("frame_1").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_2").attribute("x").as_int(), node.child("idle").child("frame_2").attribute("y").as_int(), node.child("idle").child("frame_2").attribute("w").as_int(), node.child("idle").child("frame_2").attribute("h").as_int() });
	navigating.PushBack({ node.child("idle").child("frame_2").attribute("x").as_int(), node.child("idle").child("frame_2").attribute("y").as_int(), node.child("idle").child("frame_2").attribute("w").as_int(), node.child("idle").child("frame_2").attribute("h").as_int() });

	navigating.speed = node.child("idle").attribute("speed").as_float();

	uint i = 0;
	for (pugi::xml_node arrow_node = node.child("arrow").child("frame_1"); arrow_node; arrow_node = arrow_node.next_sibling())
	{
		arrow[i].x = arrow_node.attribute("x").as_int();
		arrow[i].y = arrow_node.attribute("y").as_int();
		arrow[i].w = arrow_node.attribute("w").as_int();
		arrow[i].h = arrow_node.attribute("h").as_int();

		i++;
	}

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

	if (selected_unit != NULL && selected_unit->entity_type == UNIT && selected_unit->state == SELECTED)
		DrawArrow();
	
	return true;
}

bool Player::PostUpdate()
{
	Draw();

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
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		uint camy = abs(App->render->camera.y);
		uint scale = App->win->GetScale();
		uint width, height;
		App->win->GetWindowSize(width, height);
		camy += height;

		if (camy < (App->map->map_data.height * 16 * scale) && world_position.y >(camy / scale) - 47)
		{
			App->render->camera.y -= 16 * scale;
		}

		if (position.y < App->map->map_data.height - 1)
		{
			position.y += 1;
			App->audio->PlayFx(1);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		uint camy = abs(App->render->camera.y);
		uint scale = App->win->GetScale();

		if (camy > 0 && world_position.y + 1 < (camy / scale) + 32)
		{
			App->render->camera.y += 16 * scale;
		}

		if (position.y > 0)
		{
			position.y -= 1;
			App->audio->PlayFx(1);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		uint camx = abs(App->render->camera.x);
		uint scale = App->win->GetScale();
		uint width, height;
		App->win->GetWindowSize(width, height);
		camx += width;

		if (camx < (App->map->map_data.width * 16 * scale) && world_position.x > (camx / scale) - 47)
		{
			App->render->camera.x -= 16 * scale;
		}

		if (position.x < App->map->map_data.width - 1)
		{
			position.x += 1;
			App->audio->PlayFx(1);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		uint camx = abs(App->render->camera.x);
		uint scale = App->win->GetScale();

		if (camx > 0 && world_position.x + 1 < (camx / scale) + 32)
		{
			App->render->camera.x += 16 * scale;
		}

		if (position.x > 0)
		{
			position.x -= 1;
			App->audio->PlayFx(1);
		}
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
				
				if (selected_unit->entity_type == UNIT)
					state = UNIT_SELECTED;
				else if (selected_unit->entity_type == BUILDING)
					state = BUILDING_SELECTED;

				break;
			}
		}
		if (selected_unit == NULL)
		{
			//obrir el menú
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN && selected_unit != NULL)
	{
		if (selected_unit->entity_type == UNIT && App->map->visited.find(position) != -1)
		{
			selected_unit->GetPath(position);
			selected_unit->state = MOVING;
			this->active = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (selected_unit != NULL && selected_unit->state == SELECTED)
		{
			selected_unit->OnRelease();
			selected_unit = nullptr;
			state = NAVIGATING;
		}
		else if (selected_unit != NULL && selected_unit->state != SELECTED)
		{
			selected_unit->CancelAction();
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
	App->render->Blit(graphic, offset.x + world_position.x, offset.y + world_position.y, &r);
}

void Player::DrawArrow() const
{
	p2List_item<BFS_node>* iterator = App->map->backtrack.start;
	BFS_node node;
	iPoint prev_pos = position;

	while (iterator != NULL)
	{
		if (iterator->data.data == position)
		{
			node = iterator->data;
			break;
		}

		iterator = iterator->next;
	}

	if (node.data.x == -1)
		return;

	//final arrow
	{
		if (node.parent.x > position.x)
		{
			prev_pos = App->map->MapToWorld(position.x, position.y);
			App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_LEFT]);
		}
		else if (node.parent.x < position.x && node.parent.x != -1)
		{
			prev_pos = App->map->MapToWorld(position.x, position.y);
			App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_RIGHT]);
		}
		else if (node.parent.y > position.y)
		{
			prev_pos = App->map->MapToWorld(position.x, position.y);
			App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_UP]);
		}
		else if (node.parent.y < position.y && node.parent.y != -1)
		{
			prev_pos = App->map->MapToWorld(position.x, position.y);
			App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_DOWN]);
		}
	}

	while (node.data != selected_unit->position)
	{
		while (iterator != NULL && iterator->data.data != node.parent)
		{
			iterator = iterator->prev;

			if (iterator != NULL && iterator->data.data == node.parent)
			{
				node = iterator->data;
				break;
			}
		}
		prev_pos = App->map->WorldToMap(prev_pos.x, prev_pos.y);

		//HORIZONTAL
		if ((prev_pos.x > node.data.x && node.parent.x < node.data.x || prev_pos.x < node.data.x && node.parent.x > node.data.x) && prev_pos.y == node.parent.y)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_HORIZONTAL]);
			}
		}
		//VERTICAL
		else if ((prev_pos.y > node.data.y && node.parent.y < node.data.y || prev_pos.y < node.data.y && node.parent.y > node.data.y) && prev_pos.x == node.parent.x)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_VERTICAL]);
			}
		}
		//UP_RIGHT
		else if (prev_pos.x > node.data.x && node.parent.y > node.data.y || prev_pos.y > node.data.y && node.parent.x > node.data.x)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_UP_RIGHT]);
			}
		}
		//UP_LEFT
		else if (prev_pos.x < node.data.x && node.parent.y > node.data.y || prev_pos.y > node.data.y && node.parent.x < node.data.x)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_UP_LEFT]);
			}
		}
		//DOWN_RIGHT
		else if (prev_pos.x > node.data.x && node.parent.y < node.data.y || prev_pos.y < node.data.y && node.parent.x > node.data.x)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_DOWN_RIGHT]);
			}
		}
		//DOWN_LEFT
		else if (prev_pos.x < node.data.x && node.parent.y < node.data.y || prev_pos.y < node.data.y && node.parent.x < node.data.x)
		{
			if (node.data != selected_unit->position)
			{
				prev_pos = App->map->MapToWorld(node.data.x, node.data.y);
				App->render->Blit(graphic, prev_pos.x, prev_pos.y, &arrow[ARROW_DOWN_LEFT]);
			}
		}
	}
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