#ifndef __GUI_ELEMENT__
#define __GUI_ELEMENT__

#include "Module.h"
#include "M_Gui.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include "p2Defs.h"


struct SDL_Rect;

enum element_type
{
	BUTTON
};

enum button_type
{
	WAIT
};

enum menu_type
{
	MAINMENU,
	COMMAND
};

class GuiElement
{
public:

	GuiElement();
	virtual ~GuiElement();

	virtual void Start();

	void Update(float dt);

	virtual void Draw();

	virtual void OnClick();

	virtual void OnDrag() {};

	virtual void OnRelease() {};

	void PositionUpdate();

public:

	element_type		etype;
	menu_type			mtype;

	iPoint				position;

	bool				active;
	bool				follows_camera;

	SDL_Rect			rect;
};

#endif