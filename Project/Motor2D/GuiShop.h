#ifndef __GUI_SHOP__
#define __GUI_SHOP__

#include "p2Point.h"
#include "Application.h"
#include "GuiElement.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"

#define SHOP_OFFSET 7

struct SDL_Texture;
struct SDL_Rect;
class GuiImage;

class GuiShop : public GuiElement
{
public:

	GuiShop();
	GuiShop(iPoint position, SDL_Rect rect, menu_type mtype, bool follows_camera = false);
	~GuiShop();

	void Start();

	void Update();

	void Draw();

	void OnActivation();
	void OnDisable();

public:

	uint elements = 0;

	p2List<GuiButton*> attached_buttons;
};

#endif