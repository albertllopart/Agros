#include "GuiButton.h"
#include "M_Render.h"
#include "M_Audio.h"
#include "M_Map.h"
#include "M_Window.h"

GuiButton::GuiButton() {};

GuiButton::GuiButton(iPoint position, SDL_Rect rect, button_type btype, menu_type mtype, Module* callback, bool follows_camera) : GuiElement()
{
	this->position = position;
	this->rect = rect;
	this->btype = btype;
	this->mtype = mtype;
	this->callback = callback;
	this->follows_camera = follows_camera;

	active = false;
	selected = false;

	etype = BUTTON;
}

GuiButton::~GuiButton() {};

void GuiButton::Draw()
{
	if (active == true)
	{
		PositionUpdate();

		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		App->render->Blit(App->gui->GetAtlas(), world_position.x, world_position.y, &rect);
	}
}

void GuiButton::OnClick()
{
	callback->GuiTrigger(this);
	
	return;
}