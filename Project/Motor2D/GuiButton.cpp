#include "GuiButton.h"
#include "M_Render.h"
#include "M_Audio.h"

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
	App->render->Blit(App->gui->GetAtlas(), position.x, position.y, &rect);
}

void GuiButton::OnClick()
{
	if (mouseover == true && active == true)
	{
		callback->GuiTrigger(this);
	}
	return;
}