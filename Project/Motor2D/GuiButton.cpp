#include "GuiButton.h"
#include "M_Render.h"
#include "M_Audio.h"
#include "M_Map.h"
#include "M_Window.h"

GuiButton::GuiButton() {};

GuiButton::GuiButton(iPoint position, SDL_Rect rect, button_type btype, Module* callback, bool follows_camera)
{
	this->position = position;
	this->rect = rect;
	this->mover = mover;
	this->pressed = pressed;
	this->btype = btype;
	this->callback = callback;
	this->follows_camera = follows_camera;

	element_type = BUTTON;
}

GuiButton::~GuiButton() {};

void GuiButton::Draw()
{
	uint scale = App->win->GetScale();
	uint camx = abs(App->render->camera.x) / 16 / scale;
	uint camy = abs(App->render->camera.y) / 16 / scale;

	position.x = camx + 1;
	position.y = camy + 1;

	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	App->render->Blit(App->gui->GetAtlas(), world_position.x, world_position.y, &rect);
}

void GuiButton::OnClick()
{
	if (mouseover == true && active == true)
	{
		callback->GuiTrigger(this);
	}
	return;
}