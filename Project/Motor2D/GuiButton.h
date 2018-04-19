#ifndef __GUI_BUTTON__
#define __GUI_BUTTON__

#include "p2Point.h"
#include "Application.h"
#include "GuiElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"

struct SDL_Texture;
struct SDL_Rect;
class GuiImage;

class GuiButton : public GuiElement
{
public:

	GuiButton();
	GuiButton(iPoint position, SDL_Rect rect, SDL_Rect selected_rect, button_type btype, menu_type mtype, Module* callback, bool follows_camera = false);
	~GuiButton();

	void Draw();

	//interaction
	void OnClick();

public:

	button_type btype;

	bool selected;

	SDL_Rect selected_rect;

	Module* callback = nullptr;
	GuiImage* imgcallback = nullptr;
};

#endif