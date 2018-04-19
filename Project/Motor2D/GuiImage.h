#ifndef __GUI_IMAGE__
#define __GUI_IMAGE__

#include "p2Point.h"
#include "Application.h"
#include "GuiElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"

struct SDL_Texture;
struct SDL_Rect;

class GuiImage : public GuiElement
{
public:

	GuiImage();
	GuiImage(iPoint position, SDL_Rect rect, menu_type mtype, bool follows_camera = false);
	~GuiImage();

	void Draw();

public:

};

#endif