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
	GuiButton(iPoint position, SDL_Rect rect, button_type btype, Module* callback, bool follows_camera = false);
	~GuiButton();

	void Draw();

	//interaction
	void OnClick();

public:

	button_type btype;

	SDL_Rect mover;
	SDL_Rect pressed;

	Module* callback = nullptr;
	GuiImage* imgcallback = nullptr;
};

#endif