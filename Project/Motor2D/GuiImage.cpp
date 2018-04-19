#include "GuiImage.h"
#include "M_Render.h"
#include "M_Audio.h"
#include "M_Map.h"
#include "M_Window.h"

GuiImage::GuiImage() {};

GuiImage::GuiImage(iPoint position, SDL_Rect rect, menu_type mtype, bool follows_camera) : GuiElement()
{
	this->position = position;
	this->rect = rect;
	this->mtype = mtype;
	this->follows_camera = follows_camera;

	active = false;

	etype = IMAGE;
}

GuiImage::~GuiImage() {};

void GuiImage::Draw()
{
	if (active == true)
	{
		PositionUpdate();

		iPoint world_position = App->map->MapToWorld(position.x, position.y);
		App->render->Blit(App->gui->GetAtlas(), world_position.x, world_position.y, &rect);
	}
}