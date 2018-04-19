#include "GuiShop.h"
#include "M_Render.h"
#include "M_Audio.h"
#include "M_Map.h"
#include "M_Window.h"
#include "M_EntityManager.h"
#include "GuiButton.h"

GuiShop::GuiShop() {};

GuiShop::GuiShop(iPoint position, SDL_Rect rect, menu_type mtype, bool follows_camera) : GuiElement()
{
	this->position = position;
	this->rect = rect;
	this->mtype = mtype;
	this->follows_camera = follows_camera;

	active = false;

	etype = SHOP;
}

GuiShop::~GuiShop() {};

void GuiShop::Start()
{
	pugi::xml_node node = App->GetConfigNode("gui");

	SDL_Rect temp;
	temp.x = node.child("shop").child("buttons").child("cani").child("idle").attribute("x").as_int();
	temp.y = node.child("shop").child("buttons").child("cani").child("idle").attribute("y").as_int();
	temp.w = node.child("shop").child("buttons").child("cani").child("idle").attribute("w").as_int();
	temp.h = node.child("shop").child("buttons").child("cani").child("idle").attribute("h").as_int();

	SDL_Rect temp2;
	temp2.x = node.child("shop").child("buttons").child("cani").child("selected").attribute("x").as_int();
	temp2.y = node.child("shop").child("buttons").child("cani").child("selected").attribute("y").as_int();
	temp2.w = node.child("shop").child("buttons").child("cani").child("selected").attribute("w").as_int();
	temp2.h = node.child("shop").child("buttons").child("cani").child("selected").attribute("h").as_int();

	iPoint temp_position;
	temp_position.x = position.x + SHOP_OFFSET;
	temp_position.y = position.y + SHOP_OFFSET + (23 * elements);

	attached_buttons.add(App->gui->CreateButton(temp_position.x, temp_position.y, temp, temp2, BUY_CANI, SHOP_MENU, App->entities));

	elements++;

	//2
	temp.x = node.child("shop").child("buttons").child("cani").child("idle").attribute("x").as_int();
	temp.y = node.child("shop").child("buttons").child("cani").child("idle").attribute("y").as_int();
	temp.w = node.child("shop").child("buttons").child("cani").child("idle").attribute("w").as_int();
	temp.h = node.child("shop").child("buttons").child("cani").child("idle").attribute("h").as_int();

	temp2.x = node.child("shop").child("buttons").child("cani").child("selected").attribute("x").as_int();
	temp2.y = node.child("shop").child("buttons").child("cani").child("selected").attribute("y").as_int();
	temp2.w = node.child("shop").child("buttons").child("cani").child("selected").attribute("w").as_int();
	temp2.h = node.child("shop").child("buttons").child("cani").child("selected").attribute("h").as_int();

	temp_position.x = position.x + SHOP_OFFSET;
	temp_position.y = position.y + SHOP_OFFSET + (23 * elements);

	attached_buttons.add(App->gui->CreateButton(temp_position.x, temp_position.y, temp, temp2, BUY_CANI, SHOP_MENU, App->entities));

	elements++;
}

void GuiShop::Update()
{
	uint scale = App->win->GetScale();
	position.x = abs(App->render->camera.x) / scale + 16;
	position.y = abs(App->render->camera.y) / scale + 16;

	p2List_item<GuiButton*>* item = attached_buttons.start;
	for (int i = 0; i < elements; i++)
	{
		iPoint temp_position;
		temp_position.x = position.x + SHOP_OFFSET;
		temp_position.y = position.y + SHOP_OFFSET + (23 * i);

		item->data->position = temp_position;

		item = item->next;
	}
}

void GuiShop::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &rect);

	p2List_item<GuiButton*>* item = attached_buttons.start;
	while (item != NULL)
	{
		item->data->Draw();
		item = item->next;
	}
}

void GuiShop::OnActivation()
{
	active = true;

	p2List_item<GuiButton*>* item = attached_buttons.start;
	App->gui->selected_button = item->data;

	while (item != NULL)
	{
		App->gui->active_buttons.add(item->data);
		item = item->next;
	}
}

void GuiShop::OnDisable()
{
	active = false;

	App->gui->active_buttons.clear();
	App->gui->selected_button = nullptr;
}
