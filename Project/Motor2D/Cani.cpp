#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "Cani.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"
#include "M_Player.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "M_Input.h"

Cani::Cani() : Unit()
{
	name.create("cani");
}

Cani::~Cani()
{}

bool Cani::Awake(pugi::xml_node& config)
{
	move_range = config.child("units").child("cani").attribute("move").as_uint();

	//Animations
	pugi::xml_node node = config.child("units").child("cani").child("animation");

	idle_right.PushBack({ node.child("idle_right").child("frame_1").attribute("x").as_int(), node.child("idle_right").child("frame_1").attribute("y").as_int(), node.child("idle_right").child("frame_1").attribute("w").as_int(), node.child("idle_right").child("frame_1").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_2").attribute("x").as_int(), node.child("idle_right").child("frame_2").attribute("y").as_int(), node.child("idle_right").child("frame_2").attribute("w").as_int(), node.child("idle_right").child("frame_2").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_3").attribute("x").as_int(), node.child("idle_right").child("frame_3").attribute("y").as_int(), node.child("idle_right").child("frame_3").attribute("w").as_int(), node.child("idle_right").child("frame_3").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_4").attribute("x").as_int(), node.child("idle_right").child("frame_4").attribute("y").as_int(), node.child("idle_right").child("frame_4").attribute("w").as_int(), node.child("idle_right").child("frame_4").attribute("h").as_int(), });

	idle_right.speed = node.child("idle_right").attribute("speed").as_float();

	wait_right.PushBack({ node.child("wait_right").child("frame_1").attribute("x").as_int(), node.child("wait_right").child("frame_1").attribute("y").as_int(), node.child("wait_right").child("frame_1").attribute("w").as_int(), node.child("wait_right").child("frame_1").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_2").attribute("x").as_int(), node.child("wait_right").child("frame_2").attribute("y").as_int(), node.child("wait_right").child("frame_2").attribute("w").as_int(), node.child("wait_right").child("frame_2").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_3").attribute("x").as_int(), node.child("wait_right").child("frame_3").attribute("y").as_int(), node.child("wait_right").child("frame_3").attribute("w").as_int(), node.child("wait_right").child("frame_3").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_4").attribute("x").as_int(), node.child("wait_right").child("frame_4").attribute("y").as_int(), node.child("wait_right").child("frame_4").attribute("w").as_int(), node.child("wait_right").child("frame_4").attribute("h").as_int(), });

	wait_right.speed = node.child("wait_right").attribute("speed").as_float();

	walk_right.PushBack({ node.child("walk_right").child("frame_1").attribute("x").as_int(), node.child("walk_right").child("frame_1").attribute("y").as_int(), node.child("walk_right").child("frame_1").attribute("w").as_int(), node.child("walk_right").child("frame_1").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_2").attribute("x").as_int(), node.child("walk_right").child("frame_2").attribute("y").as_int(), node.child("walk_right").child("frame_2").attribute("w").as_int(), node.child("walk_right").child("frame_2").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_3").attribute("x").as_int(), node.child("walk_right").child("frame_3").attribute("y").as_int(), node.child("walk_right").child("frame_3").attribute("w").as_int(), node.child("walk_right").child("frame_3").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_4").attribute("x").as_int(), node.child("walk_right").child("frame_4").attribute("y").as_int(), node.child("walk_right").child("frame_4").attribute("w").as_int(), node.child("walk_right").child("frame_4").attribute("h").as_int(), });

	walk_right.speed = node.child("walk_right").attribute("speed").as_float();

	walk_left.PushBack({ node.child("walk_left").child("frame_1").attribute("x").as_int(), node.child("walk_left").child("frame_1").attribute("y").as_int(), node.child("walk_left").child("frame_1").attribute("w").as_int(), node.child("walk_left").child("frame_1").attribute("h").as_int(), });
	walk_left.PushBack({ node.child("walk_left").child("frame_2").attribute("x").as_int(), node.child("walk_left").child("frame_2").attribute("y").as_int(), node.child("walk_left").child("frame_2").attribute("w").as_int(), node.child("walk_left").child("frame_2").attribute("h").as_int(), });
	walk_left.PushBack({ node.child("walk_left").child("frame_3").attribute("x").as_int(), node.child("walk_left").child("frame_3").attribute("y").as_int(), node.child("walk_left").child("frame_3").attribute("w").as_int(), node.child("walk_left").child("frame_3").attribute("h").as_int(), });
	walk_left.PushBack({ node.child("walk_left").child("frame_4").attribute("x").as_int(), node.child("walk_left").child("frame_4").attribute("y").as_int(), node.child("walk_left").child("frame_4").attribute("w").as_int(), node.child("walk_left").child("frame_4").attribute("h").as_int(), });
		 
	walk_left.speed = node.child("walk_left").attribute("speed").as_float();

	walk_up.PushBack({ node.child("walk_up").child("frame_1").attribute("x").as_int(), node.child("walk_up").child("frame_1").attribute("y").as_int(), node.child("walk_up").child("frame_1").attribute("w").as_int(), node.child("walk_up").child("frame_1").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_2").attribute("x").as_int(), node.child("walk_up").child("frame_2").attribute("y").as_int(), node.child("walk_up").child("frame_2").attribute("w").as_int(), node.child("walk_up").child("frame_2").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_3").attribute("x").as_int(), node.child("walk_up").child("frame_3").attribute("y").as_int(), node.child("walk_up").child("frame_3").attribute("w").as_int(), node.child("walk_up").child("frame_3").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_4").attribute("x").as_int(), node.child("walk_up").child("frame_4").attribute("y").as_int(), node.child("walk_up").child("frame_4").attribute("w").as_int(), node.child("walk_up").child("frame_4").attribute("h").as_int(), });

	walk_up.speed = node.child("walk_up").attribute("speed").as_float();

	walk_down.PushBack({ node.child("walk_down").child("frame_1").attribute("x").as_int(), node.child("walk_down").child("frame_1").attribute("y").as_int(), node.child("walk_down").child("frame_1").attribute("w").as_int(), node.child("walk_down").child("frame_1").attribute("h").as_int(), });
	walk_down.PushBack({ node.child("walk_down").child("frame_2").attribute("x").as_int(), node.child("walk_down").child("frame_2").attribute("y").as_int(), node.child("walk_down").child("frame_2").attribute("w").as_int(), node.child("walk_down").child("frame_2").attribute("h").as_int(), });
	walk_down.PushBack({ node.child("walk_down").child("frame_3").attribute("x").as_int(), node.child("walk_down").child("frame_3").attribute("y").as_int(), node.child("walk_down").child("frame_3").attribute("w").as_int(), node.child("walk_down").child("frame_3").attribute("h").as_int(), });
	walk_down.PushBack({ node.child("walk_down").child("frame_4").attribute("x").as_int(), node.child("walk_down").child("frame_4").attribute("y").as_int(), node.child("walk_down").child("frame_4").attribute("w").as_int(), node.child("walk_down").child("frame_4").attribute("h").as_int(), });

	walk_down.speed = node.child("walk_down").attribute("speed").as_float();

	return true;
}

bool Cani::Start()
{
	entity_type = UNIT;
	unit_type = CANI;

	graphic = App->tex->Load("textures/cani.png");
	current_animation = &idle_right;

	state = WAITING_TURN;
	direction = RIGHT;

	current_moving_position.x = position.x * 16;
	current_moving_position.y = position.y * 16;

	prev_position = position;

	return true;
}

bool Cani::Update(float dt)
{
	if (state == MOVING)
	{
		Move(dt);
	}

	Draw();

	return true;
}

bool Cani::CleanUp()
{
	return true;
}

void Cani::Draw()
{
	switch (state)
	{
		case IDLE:
		{
			current_animation = &idle_right;
			break;
		}
		case SELECTED:
		{
			current_animation = &walk_right;
			break;
		}
		case MOVING:
		{
			if (direction == RIGHT)
			{
				current_animation = &walk_right;
				break;
			}
			else if (direction == UP)
			{
				current_animation = &walk_up;
				break;
			}
			else if (direction == DOWN)
			{
				current_animation = &walk_down;
				break;
			}
			else if (direction == LEFT)
			{
				current_animation = &walk_left;
				break;
			}
		}
		case WAITING_TURN:
		{
			current_animation = &wait_right;
		}
	}

	SDL_Rect r = current_animation->GetCurrentFrame();
	App->render->Blit(graphic, current_moving_position.x, current_moving_position.y + OFFSET, &r);
}

iPoint Cani::GetPosition() const
{
	return position;
}

bool Cani::OnSelection()
{
	state = SELECTED;

	//BFS
	App->map->ResetBFS(position);
	App->map->PropagateBFS(this);

	return true;
}

bool Cani::OnRelease()
{
	state = IDLE;

	return true;
}

bool Cani::OnWait()
{
	state = WAITING_TURN;
	App->player->selected_unit = nullptr;
	App->player->active = true;

	return true;
}

void Cani::GetPath(iPoint goal)
{
	if (path.count() > 0)
	{
		path.clear();
	}

	this->goal = goal;

	if (App->map->backtrack.count() > 0)
	{
		p2List_item<BFS_node>* iterator = App->map->backtrack.start;
		BFS_node node;

		while (iterator != NULL)
		{
			if (iterator->data.data == goal)
			{
				node = iterator->data;
				break;
			}
			iterator = iterator->next;
		}

		while (node.data != position)
		{
			path.add(node.data);

			while (iterator != NULL)
			{
				if (iterator->data.data == node.parent)
				{
					node = iterator->data;
					break;
				}
				iterator = iterator->prev;
			}
		}
	}
}

void Cani::Move(float dt)
{
	if (position != goal && path.count() > 0)
	{
		p2List_item<iPoint>* node = path.end;
		iPoint check_left_up = App->map->WorldToMap(current_moving_position.x + 15, current_moving_position.y + 15);
		iPoint check_right_down = App->map->WorldToMap(current_moving_position.x, current_moving_position.y);
		
		if (node != NULL)
		{
			if (check_left_up.x > node->data.x)
			{
				current_moving_position.x -= 4;
				direction = LEFT;
			}
			else if (check_right_down.x < node->data.x)
			{
				current_moving_position.x += 4;
				direction = RIGHT;
			}
			else if (check_left_up.y > node->data.y)
			{
				current_moving_position.y -= 4;
				direction = UP;
			}
			else if (check_right_down.y < node->data.y)
			{
				current_moving_position.y += 4;
				direction = DOWN;
			}

			iPoint check_left_up_2 = App->map->WorldToMap(current_moving_position.x + 15, current_moving_position.y + 15);
			iPoint check_right_down_2 = App->map->WorldToMap(current_moving_position.x, current_moving_position.y);

			if (direction == LEFT || direction == UP)
			{
				if (check_left_up_2 == node->data)
				{
					position.x = check_left_up_2.x;
					position.y = check_left_up_2.y;
					LOG("position: %d %d", position.x, position.y);
					path.del(node);
				}
			}
			else if (direction == RIGHT || direction == DOWN)
			{
				if (check_right_down_2 == node->data)
				{
					position.x = check_right_down_2.x;
					position.y = check_right_down_2.y;
					LOG("position: %d %d", position.x, position.y);
					path.del(node);
				}
			}
		}	
	}

	if (position == goal)
	{
		state = WAITING_COMMAND;
		App->input->state = UI_INPUT;
		App->gui->ActivateMenu(COMMAND_MENU);
	}
}

void Cani::CancelAction()
{
	position = prev_position;
	current_moving_position.x = position.x * 16;
	current_moving_position.y = position.y * 16;
	state = SELECTED;

	App->map->ResetBFS(position);
	App->map->PropagateBFS(this);
}