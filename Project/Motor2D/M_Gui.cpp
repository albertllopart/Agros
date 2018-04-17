#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Input.h"
#include "M_EntityManager.h"
#include "M_Map.h"
#include "M_Audio.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "GuiButton.h"
#include "M_Window.h"

Gui::Gui() : Module()
{
	name.create("gui");
}

// Destructor
Gui::~Gui()
{
	App->tex->UnLoad(atlas);
}

// Called before render is available
bool Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	uint scale = App->win->GetScale();
	uint width, height;
	App->win->GetWindowSize(width, height);
	uint camx = (App->render->camera.x / 16) / scale;
	uint camy = (App->render->camera.y / 16) / scale;

	SDL_Rect temp;
	temp.x = conf.child("buttons").child("command_button").attribute("x").as_int();
	temp.y = conf.child("buttons").child("command_button").attribute("y").as_int();
	temp.w = conf.child("buttons").child("command_button").attribute("w").as_int();
	temp.h = conf.child("buttons").child("command_button").attribute("h").as_int();

	CreateButton(0, 0, temp, WAIT, App->entities, true);

	return ret;
}

// Called before the first frame
bool Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool Gui::PostUpdate()
{
	p2List_item<GuiElement*>* element = elements.start;
	while (element != NULL)
	{
		element->data->Draw();
		element = element->next;
	}
	return true;
}

// Called before quitting
bool Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
SDL_Texture* Gui::GetAtlas() const
{
	return atlas;
}

GuiButton* Gui::CreateButton(int x, int y, SDL_Rect rect, button_type btype, Module* callback, bool follows_camera)
{
	iPoint position = { x,y };
	GuiButton* item = new GuiButton(position, rect, btype, callback, follows_camera);

	elements.add(item);

	return item;
}

void Gui::DeleteElement(GuiElement* element)
{
	if (element != nullptr)
	{
		int find = elements.find(element);
		int current_position = 0;
		p2List_item<GuiElement*>* item = elements.start;
		while (item != NULL)
		{
			if (current_position == find)
			{
				elements.del(item);
				return;
			}
			item = item->next;
			current_position++;
		}
	}
}

bool Gui::MouseOnRect(SDL_Rect rect)
{
	int x, y;
	App->input->GetMousePosition(x, y);

	if (x < (rect.x + rect.w) && x > rect.x &&y < (rect.y + rect.h) && y > rect.y)
	{
		return true;
	}
	else
		return false;
}

void Gui::GuiTrigger(GuiElement* element)
{

}

// class Gui ---------------------------------------------------

