#include "Cani.h"
#include "M_Textures.h"
#include "M_Render.h"
#include "M_Map.h"

Cani::Cani() : Entity()
{
	name.create("cani");
}

Cani::~Cani()
{}

bool Cani::Awake(pugi::xml_node& config)
{
	pugi::xml_node node = config.child("units").child("cani").child("animation");

	idle_right.PushBack({ node.child("idle_right").child("frame_1").attribute("x").as_int(), node.child("idle_right").child("frame_1").attribute("y").as_int(), node.child("idle_right").child("frame_1").attribute("w").as_int(), node.child("idle_right").child("frame_1").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_2").attribute("x").as_int(), node.child("idle_right").child("frame_2").attribute("y").as_int(), node.child("idle_right").child("frame_2").attribute("w").as_int(), node.child("idle_right").child("frame_2").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_3").attribute("x").as_int(), node.child("idle_right").child("frame_3").attribute("y").as_int(), node.child("idle_right").child("frame_3").attribute("w").as_int(), node.child("idle_right").child("frame_3").attribute("h").as_int(), });
	idle_right.PushBack({ node.child("idle_right").child("frame_4").attribute("x").as_int(), node.child("idle_right").child("frame_4").attribute("y").as_int(), node.child("idle_right").child("frame_4").attribute("w").as_int(), node.child("idle_right").child("frame_4").attribute("h").as_int(), });

	idle_right.speed = node.child("idle_right").attribute("speed").as_float();

	walk_right.PushBack({ node.child("walk_right").child("frame_1").attribute("x").as_int(), node.child("walk_right").child("frame_1").attribute("y").as_int(), node.child("walk_right").child("frame_1").attribute("w").as_int(), node.child("walk_right").child("frame_1").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_2").attribute("x").as_int(), node.child("walk_right").child("frame_2").attribute("y").as_int(), node.child("walk_right").child("frame_2").attribute("w").as_int(), node.child("walk_right").child("frame_2").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_3").attribute("x").as_int(), node.child("walk_right").child("frame_3").attribute("y").as_int(), node.child("walk_right").child("frame_3").attribute("w").as_int(), node.child("walk_right").child("frame_3").attribute("h").as_int(), });
	walk_right.PushBack({ node.child("walk_right").child("frame_4").attribute("x").as_int(), node.child("walk_right").child("frame_4").attribute("y").as_int(), node.child("walk_right").child("frame_4").attribute("w").as_int(), node.child("walk_right").child("frame_4").attribute("h").as_int(), });

	walk_right.speed = node.child("walk_right").attribute("speed").as_float();

	walk_up.PushBack({ node.child("walk_up").child("frame_1").attribute("x").as_int(), node.child("walk_up").child("frame_1").attribute("y").as_int(), node.child("walk_up").child("frame_1").attribute("w").as_int(), node.child("walk_up").child("frame_1").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_2").attribute("x").as_int(), node.child("walk_up").child("frame_2").attribute("y").as_int(), node.child("walk_up").child("frame_2").attribute("w").as_int(), node.child("walk_up").child("frame_2").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_3").attribute("x").as_int(), node.child("walk_up").child("frame_3").attribute("y").as_int(), node.child("walk_up").child("frame_3").attribute("w").as_int(), node.child("walk_up").child("frame_3").attribute("h").as_int(), });
	walk_up.PushBack({ node.child("walk_up").child("frame_4").attribute("x").as_int(), node.child("walk_up").child("frame_4").attribute("y").as_int(), node.child("walk_up").child("frame_4").attribute("w").as_int(), node.child("walk_up").child("frame_4").attribute("h").as_int(), });

	walk_up.speed = node.child("walk_right").attribute("speed").as_float();

	return true;
}

bool Cani::Start()
{
	type = CANI;
	graphic = App->tex->Load("textures/cani.png");
	current_animation = &idle_right;
	state = IDLE;
	direction = RIGHT;

	return true;
}

bool Cani::Update(float dt)
{
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
			if (direction == RIGHT)
			{
				current_animation = &idle_right;
				break;
			}
		}
		case SELECTED:
		{
			if (direction == RIGHT)
			{
				current_animation = &walk_right;
				break;
			}
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
		}
	}

	SDL_Rect r = current_animation->GetCurrentFrame();
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(graphic, world_position.x, world_position.y + OFFSET, &r);
}

iPoint Cani::GetPosition() const
{
	return position;
}

bool Cani::OnSelection()
{
	state = SELECTED;

	return true;
}

bool Cani::OnRelease()
{
	state = IDLE;

	return true;
}