#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Input.h"
#include "M_EntityManager.h"
#include "Entity.h"
#include "M_Map.h"
#include "M_Audio.h"
#include "M_Gui.h"
#include "GuiElement.h"
#include "GuiButton.h"
#include "GuiShop.h"
#include "M_Window.h"
#include "M_Player.h"
#include "M_Scene.h"

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

	//WAIT BUTTON
	SDL_Rect temp;
	temp.x = conf.child("command").child("buttons").child("wait").child("idle").attribute("x").as_int();
	temp.y = conf.child("command").child("buttons").child("wait").child("idle").attribute("y").as_int();
	temp.w = conf.child("command").child("buttons").child("wait").child("idle").attribute("w").as_int();
	temp.h = conf.child("command").child("buttons").child("wait").child("idle").attribute("h").as_int();

	SDL_Rect temp2;
	temp2.x = conf.child("command").child("buttons").child("wait").child("selected").attribute("x").as_int();
	temp2.y = conf.child("command").child("buttons").child("wait").child("selected").attribute("y").as_int();
	temp2.w = conf.child("command").child("buttons").child("wait").child("selected").attribute("w").as_int();
	temp2.h = conf.child("command").child("buttons").child("wait").child("selected").attribute("h").as_int();

	CreateButton(0, 0, temp, temp2, WAIT, COMMAND_MENU, App->entities, true);

	//ATTACK BUTTON
	temp.x = conf.child("command").child("buttons").child("attack").child("idle").attribute("x").as_int();
	temp.y = conf.child("command").child("buttons").child("attack").child("idle").attribute("y").as_int();
	temp.w = conf.child("command").child("buttons").child("attack").child("idle").attribute("w").as_int();
	temp.h = conf.child("command").child("buttons").child("attack").child("idle").attribute("h").as_int();

	temp2.x = conf.child("command").child("buttons").child("attack").child("selected").attribute("x").as_int();
	temp2.y = conf.child("command").child("buttons").child("attack").child("selected").attribute("y").as_int();
	temp2.w = conf.child("command").child("buttons").child("attack").child("selected").attribute("w").as_int();
	temp2.h = conf.child("command").child("buttons").child("attack").child("selected").attribute("h").as_int();

	CreateButton(0, 0, temp, temp2, ATTACK, COMMAND_MENU, App->entities, true);

	//QUIT BUTTON
	temp.x = conf.child("ingame_options").child("buttons").child("quit").child("idle").attribute("x").as_int();
	temp.y = conf.child("ingame_options").child("buttons").child("quit").child("idle").attribute("y").as_int();
	temp.w = conf.child("ingame_options").child("buttons").child("quit").child("idle").attribute("w").as_int();
	temp.h = conf.child("ingame_options").child("buttons").child("quit").child("idle").attribute("h").as_int();

	temp2.x = conf.child("ingame_options").child("buttons").child("quit").child("selected").attribute("x").as_int();
	temp2.y = conf.child("ingame_options").child("buttons").child("quit").child("selected").attribute("y").as_int();
	temp2.w = conf.child("ingame_options").child("buttons").child("quit").child("selected").attribute("w").as_int();
	temp2.h = conf.child("ingame_options").child("buttons").child("quit").child("selected").attribute("h").as_int();

	CreateButton(0, 0, temp, temp2, QUIT, INGAME_OPTIONS_MENU, App->scene, true);

	//END TURN BUTTON
	temp.x = conf.child("ingame_options").child("buttons").child("end_turn").child("idle").attribute("x").as_int();
	temp.y = conf.child("ingame_options").child("buttons").child("end_turn").child("idle").attribute("y").as_int();
	temp.w = conf.child("ingame_options").child("buttons").child("end_turn").child("idle").attribute("w").as_int();
	temp.h = conf.child("ingame_options").child("buttons").child("end_turn").child("idle").attribute("h").as_int();

	temp2.x = conf.child("ingame_options").child("buttons").child("end_turn").child("selected").attribute("x").as_int();
	temp2.y = conf.child("ingame_options").child("buttons").child("end_turn").child("selected").attribute("y").as_int();
	temp2.w = conf.child("ingame_options").child("buttons").child("end_turn").child("selected").attribute("w").as_int();
	temp2.h = conf.child("ingame_options").child("buttons").child("end_turn").child("selected").attribute("h").as_int();

	CreateButton(0, 0, temp, temp2, END_TURN, INGAME_OPTIONS_MENU, App->entities, true);

	//SHOP
	temp.x = conf.child("shop").child("images").child("background").attribute("x").as_int();
	temp.y = conf.child("shop").child("images").child("background").attribute("y").as_int();
	temp.w = conf.child("shop").child("images").child("background").attribute("w").as_int();
	temp.h = conf.child("shop").child("images").child("background").attribute("h").as_int();

	CreateShop(0, 0, temp, CANI_SHOP_MENU, true);
	CreateShop(0, 0, temp, HIPSTER_SHOP_MENU, true);

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
	p2List_item<GuiElement*>* element = elements.start;
	while (element != NULL)
	{
		element->data->Update();

		element = element->next;
	}
	
	return true;
}

// Called after all Updates
bool Gui::PostUpdate()
{
	if (App->input->state == UI_INPUT)
		Input();

	p2List_item<GuiElement*>* element = elements.start;
	while (element != NULL)
	{
		if (element->data->active == true)
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

GuiButton* Gui::CreateButton(int x, int y, SDL_Rect rect, SDL_Rect selected_rect, button_type btype, menu_type mtype, Module* callback, bool follows_camera)
{
	iPoint position = { x,y };
	GuiButton* item = new GuiButton(position, rect, selected_rect, btype, mtype, callback, follows_camera);

	elements.add(item);

	return item;
}

GuiShop* Gui::CreateShop(int x, int y, SDL_Rect rect, menu_type mtype, bool follows_camera)
{
	iPoint position = { x,y };
	GuiShop* item = new GuiShop(position, rect, mtype, follows_camera);
	item->Start();

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

void Gui::ActivateMenu(menu_type mtype)
{
	switch (mtype)
	{
		case COMMAND_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == BUTTON)
				{
					GuiButton* button = (GuiButton*)item->data;

					if (button->btype == ATTACK)
					{
						Unit* unit = (Unit*)App->player->selected_unit;

						if (unit->targets.start != NULL)
						{
							button->active = true;
							active_buttons.add(button);

							if (active_elements == 0)
							{
								selected_button = button;
							}
							active_elements++;
						}
					}
				}
				item = item->next;
			}

			item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == BUTTON)
				{
					GuiButton* button = (GuiButton*)item->data;

					if (button->btype == WAIT)
					{
						button->active = true;
						active_buttons.add(button);

						if (active_elements == 0)
						{
							selected_button = button;
						}
						active_elements++;
					}
				}
				item = item->next;
			}

			UpdateActiveButtonsPosition();
			state = GUI_COMMAND;
			break;
		}

		case CANI_SHOP_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == SHOP && item->data->mtype == CANI_SHOP_MENU)
				{
					GuiShop* shop = (GuiShop*)item->data;
					shop->OnActivation();
					break;
				}
				item = item->next;
			}
			state = GUI_SHOP;
			break;
		}

		case HIPSTER_SHOP_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == SHOP && item->data->mtype == HIPSTER_SHOP_MENU)
				{
					GuiShop* shop = (GuiShop*)item->data;
					shop->OnActivation();
					break;
				}
				item = item->next;
			}
			state = GUI_SHOP;
			break;
		}

		case INGAME_OPTIONS_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == BUTTON)
				{
					GuiButton* button = (GuiButton*)item->data;
					
					if (button->mtype == INGAME_OPTIONS_MENU)
					{
						button->active = true;
						active_buttons.add(button);

						if (active_elements == 0)
						{
							selected_button = button;
						}
						active_elements++;
					}
				}
				item = item->next;
			}
			UpdateActiveButtonsPosition();
			state = GUI_INGAME_OPTIONS;
			break;
		}
	}
}

void Gui::DisableMenu(menu_type mtype)
{
	switch (mtype)
	{
		case COMMAND_MENU:
		{
			p2List_item<GuiButton*>* button = active_buttons.start;

			while (button != NULL)
			{
				button->data->active = false;
				button = button->next;
			}
			active_buttons.clear();
			selected_button = nullptr;
			active_elements = 0;
			break;
		}

		case CANI_SHOP_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == SHOP && item->data->mtype == CANI_SHOP_MENU)
				{
					GuiShop* shop = (GuiShop*)item->data;
					shop->OnDisable();
					break;
				}
				item = item->next;
			}
			break;
		}

		case HIPSTER_SHOP_MENU:
		{
			p2List_item<GuiElement*>* item = elements.start;

			while (item != NULL)
			{
				if (item->data->etype == SHOP && item->data->mtype == HIPSTER_SHOP_MENU)
				{
					GuiShop* shop = (GuiShop*)item->data;
					shop->OnDisable();
					break;
				}
				item = item->next;
			}
			break;
		}

		case INGAME_OPTIONS_MENU:
		{
			p2List_item<GuiButton*>* button = active_buttons.start;

			while (button != NULL)
			{
					button->data->active = false;
					button = button->next;
			}
			active_buttons.clear();
			selected_button = nullptr;
			active_elements = 0;
			break;
		}
	}
}

void Gui::Input()
{
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		if(selected_button != NULL)
			selected_button->OnClick();

		App->audio->PlayFx(2);
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		if (state == GUI_COMMAND)
		{
			DisableMenu(COMMAND_MENU);
			App->player->selected_unit->CancelAction();
			App->input->state = PLAYER_INPUT;
			App->player->active = true;
		}
		else if (state == GUI_SHOP)
		{
			DisableMenu(CANI_SHOP_MENU);
			DisableMenu(HIPSTER_SHOP_MENU);
			App->player->selected_unit->OnRelease();
			App->input->state = PLAYER_INPUT;
			App->player->active = true;
		}
		else if (state == GUI_INGAME_OPTIONS)
		{
			DisableMenu(INGAME_OPTIONS_MENU);
			App->input->state = PLAYER_INPUT;
			App->player->active = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		if (active_buttons.count() > 1)
		{
			int button_position = active_buttons.find(selected_button);
			p2List_item<GuiButton*>* item = active_buttons.start;

			for (int i = 0; i < button_position; i++)
			{
				item = item->next;
			}

			if (item->next != NULL)
				selected_button = item->next->data;
			else
				selected_button = active_buttons.start->data;

			App->audio->PlayFx(1);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		if (active_buttons.count() > 1)
		{
			int button_position = active_buttons.find(selected_button);
			p2List_item<GuiButton*>* item = active_buttons.start;

			for (int i = 0; i < button_position; i++)
			{
				item = item->next;
			}

			if (item->prev != NULL)
				selected_button = item->prev->data;
			else
				selected_button = active_buttons.end->data;

			App->audio->PlayFx(1);
		}
	}
}

void Gui::UpdateActiveButtonsPosition()
{
	p2List_item<GuiButton*>* button = active_buttons.start;
	int i = 0;

	while (button != NULL)
	{
		uint scale = App->win->GetScale();
		iPoint temp_position;
		temp_position.x = abs(App->render->camera.x) / scale + 16;
		temp_position.y = abs(App->render->camera.y) / scale + 16;

		button->data->position.x = temp_position.x;
		button->data->position.y = temp_position.y + (button->data->rect.h - 1) * i++;

		button = button->next;
	}
}

// class Gui ---------------------------------------------------

