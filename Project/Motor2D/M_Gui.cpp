#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Textures.h"
//#include "j1Fonts.h"
#include "M_Input.h"
#include "M_EntityManager.h"
#include "M_Map.h"
#include "M_Audio.h"
#include "M_Gui.h"

//#include "GuiElement.h"
//#include "GuiImage.h"
//#include "GuiButton.h"
//#include "GuiText.h"
//#include "GuiNumber.h"
//#include "GuiSlider.h"



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

/*GuiImage* Gui::CreateImage(int x, int y, SDL_Rect rect, menu_type mtype, uint index, bool follows_camera)
{
	iPoint position = { x,y };
	GuiImage* item = new GuiImage(position, rect, mtype, index, follows_camera);

	elements.add(item);

	return item;
}

GuiButton* j1Gui::CreateButton(int x, int y, SDL_Rect rect, SDL_Rect mover, SDL_Rect pressed, button_type btype, menu_type mtype, j1Module* callback, bool follows_camera)
{
	iPoint position = { x,y };
	GuiButton* item = new GuiButton(position, rect, mover, pressed, btype, mtype, callback, follows_camera);

	elements.add(item);

	return item;
}

GuiElement* j1Gui::CreateText(int x, int y, char* string, SDL_Color color, _TTF_Font* font, menu_type mtype, bool follows_camera)
{
	iPoint position = { x,y };
	GuiElement* item = new GuiText(position, string, color, font, mtype, follows_camera);

	elements.add(item);

	return item;
}

GuiElement* j1Gui::CreateNumber(int x, int y, uint* number, SDL_Color color, _TTF_Font* font, menu_type mtype, bool follows_camera)
{
	iPoint position = { x,y };
	GuiElement* item = new GuiNumber(position, number, color, font, mtype, follows_camera);

	elements.add(item);

	return item;
}

GuiElement* j1Gui::CreateSlider(int x, int y, SDL_Rect rect, SDL_Rect bar_rect, slider_type stype, menu_type mtype, j1Module* callback, bool follows_camera)
{
	iPoint position = { x,y };
	GuiElement* item = new GuiSlider(position, rect, bar_rect, stype, mtype, callback, follows_camera);

	elements.add(item);

	return item;
}

void j1Gui::DeleteElement(GuiElement* element)
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
}*/

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

bool Gui::GuiTrigger(GuiElement* element)
{
	return true;
}

// class Gui ---------------------------------------------------

