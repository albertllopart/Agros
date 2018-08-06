#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "Infantry.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"
#include "Entity.h"
#include "M_Player.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "M_Input.h"
#include "M_EntityManager.h"

Infantry::Infantry() : Unit()
{
	name.create("infantry");
}

Infantry::~Infantry()
{}

bool Infantry::Awake(pugi::xml_node& config)
{
	//stats

	hitpoints = config.child("units").attribute("hp").as_uint();
	move_range = config.child("units").child("infantry").attribute("move").as_uint();

	//Animations
	pugi::xml_node node = config.child("units").child("infantry").child("animation").child("hipster");

	if (entity_army == CANI)
		node = config.child("units").child("infantry").child("animation").child("cani");
		
	idle_right.PushBack({ node.child("idle_right").child("frame_1").attribute("x").as_int(), node.child("idle_right").child("frame_1").attribute("y").as_int(), node.child("idle_right").child("frame_1").attribute("w").as_int(), node.child("idle_right").child("frame_1").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_2").attribute("x").as_int(), node.child("idle_right").child("frame_2").attribute("y").as_int(), node.child("idle_right").child("frame_2").attribute("w").as_int(), node.child("idle_right").child("frame_2").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_3").attribute("x").as_int(), node.child("idle_right").child("frame_3").attribute("y").as_int(), node.child("idle_right").child("frame_3").attribute("w").as_int(), node.child("idle_right").child("frame_3").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_4").attribute("x").as_int(), node.child("idle_right").child("frame_4").attribute("y").as_int(), node.child("idle_right").child("frame_4").attribute("w").as_int(), node.child("idle_right").child("frame_4").attribute("h").as_int(), });

	idle_right.speed = node.child("idle_right").attribute("speed").as_float();

	idle_left.PushBack({ node.child("idle_left").child("frame_1").attribute("x").as_int(), node.child("idle_left").child("frame_1").attribute("y").as_int(), node.child("idle_left").child("frame_1").attribute("w").as_int(), node.child("idle_left").child("frame_1").attribute("h").as_int(), });
	idle_left.PushBack({ node.child("idle_left").child("frame_2").attribute("x").as_int(), node.child("idle_left").child("frame_2").attribute("y").as_int(), node.child("idle_left").child("frame_2").attribute("w").as_int(), node.child("idle_left").child("frame_2").attribute("h").as_int(), });
	idle_left.PushBack({ node.child("idle_left").child("frame_3").attribute("x").as_int(), node.child("idle_left").child("frame_3").attribute("y").as_int(), node.child("idle_left").child("frame_3").attribute("w").as_int(), node.child("idle_left").child("frame_3").attribute("h").as_int(), });
	idle_left.PushBack({ node.child("idle_left").child("frame_4").attribute("x").as_int(), node.child("idle_left").child("frame_4").attribute("y").as_int(), node.child("idle_left").child("frame_4").attribute("w").as_int(), node.child("idle_left").child("frame_4").attribute("h").as_int(), });

	idle_left.speed = node.child("idle_left").attribute("speed").as_float();

	wait_right.PushBack({ node.child("wait_right").child("frame_1").attribute("x").as_int(), node.child("wait_right").child("frame_1").attribute("y").as_int(), node.child("wait_right").child("frame_1").attribute("w").as_int(), node.child("wait_right").child("frame_1").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_2").attribute("x").as_int(), node.child("wait_right").child("frame_2").attribute("y").as_int(), node.child("wait_right").child("frame_2").attribute("w").as_int(), node.child("wait_right").child("frame_2").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_3").attribute("x").as_int(), node.child("wait_right").child("frame_3").attribute("y").as_int(), node.child("wait_right").child("frame_3").attribute("w").as_int(), node.child("wait_right").child("frame_3").attribute("h").as_int(), });
	wait_right.PushBack({ node.child("wait_right").child("frame_4").attribute("x").as_int(), node.child("wait_right").child("frame_4").attribute("y").as_int(), node.child("wait_right").child("frame_4").attribute("w").as_int(), node.child("wait_right").child("frame_4").attribute("h").as_int(), });

	wait_right.speed = node.child("wait_right").attribute("speed").as_float();

	wait_left.PushBack({ node.child("wait_left").child("frame_1").attribute("x").as_int(), node.child("wait_left").child("frame_1").attribute("y").as_int(), node.child("wait_left").child("frame_1").attribute("w").as_int(), node.child("wait_left").child("frame_1").attribute("h").as_int(), });
	wait_left.PushBack({ node.child("wait_left").child("frame_2").attribute("x").as_int(), node.child("wait_left").child("frame_2").attribute("y").as_int(), node.child("wait_left").child("frame_2").attribute("w").as_int(), node.child("wait_left").child("frame_2").attribute("h").as_int(), });
	wait_left.PushBack({ node.child("wait_left").child("frame_3").attribute("x").as_int(), node.child("wait_left").child("frame_3").attribute("y").as_int(), node.child("wait_left").child("frame_3").attribute("w").as_int(), node.child("wait_left").child("frame_3").attribute("h").as_int(), });
	wait_left.PushBack({ node.child("wait_left").child("frame_4").attribute("x").as_int(), node.child("wait_left").child("frame_4").attribute("y").as_int(), node.child("wait_left").child("frame_4").attribute("w").as_int(), node.child("wait_left").child("frame_4").attribute("h").as_int(), });

	wait_left.speed = node.child("wait_left").attribute("speed").as_float();

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

bool Infantry::Start()
{
	entity_type = UNIT;
	unit_type = INFANTRY;

	graphic = App->tex->Load("textures/infantry.png");
	current_animation = &idle_right;

	state = WAITING_TURN;

	if (entity_army == CANI)
		direction = RIGHT;
	else if (entity_army == HIPSTER)
		direction = LEFT;

	current_moving_position.x = position.x * 16;
	current_moving_position.y = position.y * 16;

	prev_position = position;

	return true;
}

bool Infantry::Update(float dt)
{
	if (state == MOVING)
	{
		Move(dt);
	}

	if (targeted_unit != NULL)
	{
		if (targeted_unit->position.y < position.y)
		{
			direction = UP;
		}
		else if (targeted_unit->position.y > position.y)
		{
			direction = DOWN;
		}
		else if (targeted_unit->position.x > position.x)
		{
			direction = RIGHT;
		}
		else if (targeted_unit->position.x < position.x)
		{
			direction = LEFT;
		}
	}

	Draw();

	return true;
}

bool Infantry::CleanUp()
{
	return true;
}

void Infantry::Draw()
{
	switch (state)
	{
		case IDLE:
		{
			if (direction == RIGHT)
				current_animation = &idle_right;
			else if (direction == LEFT)
				current_animation = &idle_left;
			break;
		}
		case SELECTED:
		{
			if (direction == RIGHT)
				current_animation = &walk_right;
			else if (direction == LEFT)
				current_animation = &walk_left;
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

		case WAITING_COMMAND:
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
			if (direction == RIGHT)
				current_animation = &wait_right;
			else if (direction == LEFT)
				current_animation = &wait_left;
			break;
		}
	}

	SDL_Rect r = current_animation->GetCurrentFrame();
	App->render->Blit(graphic, current_moving_position.x, current_moving_position.y + OFFSET, &r);

	if (hitpoints < 5 && hitpoints > 0 && state != MOVING)
	{
		switch (hitpoints)
		{
			case 1:
			{
				r = App->player->hp[0];
				break;
			}
			case 2:
			{
				r = App->player->hp[1];
				break;
			}
			case 3:
			{
				r = App->player->hp[2];
				break;
			}
			case 4:
			{
				r = App->player->hp[3];
				break;
			}
		}
		
		App->render->Blit(App->player->graphic, current_moving_position.x, current_moving_position.y + OFFSET + 10, &r);
	}
}

iPoint Infantry::GetPosition() const
{
	return position;
}

bool Infantry::OnSelection()
{
	state = SELECTED;

	if (entity_army == CANI)
		App->map->RewriteWalkability(CANI, HIPSTER);
	else if (entity_army == HIPSTER)
		App->map->RewriteWalkability(HIPSTER, CANI);

	//BFS
	App->map->ResetBFS(position);
	App->map->PropagateBFS(this);

	return true;
}

bool Infantry::OnRelease()
{
	state = IDLE;

	return true;
}

bool Infantry::OnWait()
{
	state = WAITING_TURN;
	prev_position = position;

	ClearTargets();

	if (entity_army == CANI)
		direction = RIGHT;
	else if (entity_army == HIPSTER)
		direction = LEFT;

	App->player->selected_unit = nullptr;
	App->player->active = true;

	return true;
}

bool Infantry::OnAttack()
{
	targeted_unit->hitpoints--;

	if (targeted_unit->hitpoints < 1)
	{
		targeted_unit->OnDying();
	}

	OnWait();

	return true;
}

bool Infantry::OnDying()
{
	//rewrite walkability
	App->map->RewriteTile(position);
	
	App->entities->DeleteEntity(this);

	return true;
}

void Infantry::GetPath(iPoint goal)
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

void Infantry::Move(float dt)
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
		SearchTargets();
		state = WAITING_COMMAND;
		App->input->state = UI_INPUT;
		App->gui->ActivateMenu(COMMAND_MENU);
	}
}

void Infantry::SearchTargets()
{
	for (p2List_item<Unit*>* unit = App->entities->units.start; unit; unit = unit->next)
	{
		if (unit->data->position.x == position.x && unit->data->position.y == position.y - 1)//north
		{
			targets.add(unit->data);
		}
	}
	for (p2List_item<Unit*>* unit = App->entities->units.start; unit; unit = unit->next)
	{
		if (unit->data->position.x == position.x + 1 && unit->data->position.y == position.y)//east
		{
			targets.add(unit->data);
		}
	}
	for (p2List_item<Unit*>* unit = App->entities->units.start; unit; unit = unit->next)
	{
		if (unit->data->position.x == position.x && unit->data->position.y == position.y + 1)//south
		{
			targets.add(unit->data);
		}
	}
	for (p2List_item<Unit*>* unit = App->entities->units.start; unit; unit = unit->next)
	{
		if (unit->data->position.x == position.x - 1 && unit->data->position.y == position.y)//west
		{
			targets.add(unit->data);
		}
	}
}

void Infantry::ClearTargets()
{
	targets.clear();
	targeted_unit = nullptr;
}

void Infantry::CancelAction()
{
	position = prev_position;
	current_moving_position.x = position.x * 16;
	current_moving_position.y = position.y * 16;
	state = SELECTED;

	ClearTargets();

	App->map->ResetBFS(position);
	App->map->PropagateBFS(this);
}