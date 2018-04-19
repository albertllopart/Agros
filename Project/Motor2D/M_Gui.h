#ifndef __M_GUI_H__
#define __M_GUI_H__ 

#include "Application.h"
#include "Module.h"
#include "p2List.h"

#define CURSOR_WIDTH 2

struct SDL_Rect;
struct SDL_Texture;
enum menu_type;
enum button_type;
enum slider_type;
class GuiElement;
class GuiButton;

enum GUI_state
{
	GUI_COMMAND,
	GUI_SHOP
};

class Gui : public Module
{
public:

	Gui();
	virtual ~Gui();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();


	SDL_Texture* GetAtlas() const;

	bool MouseOnRect(SDL_Rect rect);

	void GuiTrigger(GuiElement* element);

	GuiButton* CreateButton(int x, int y, SDL_Rect rect, button_type btype, menu_type mtype, Module* callback, bool follows_camera = false);

	void DeleteElement(GuiElement* element);

	//menu control
	void ActivateMenu(menu_type mtype);

	void DisableMenu(menu_type mtype);

	void Input();

public:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<GuiElement*> elements;
	p2List<GuiButton*> command_buttons;

	uint active_elements = 0;

	GUI_state state;
};

#endif