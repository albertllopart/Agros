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

	GuiButton* CreateButton(int x, int y, SDL_Rect rect, button_type btype, Module* callback, bool follows_camera = false);

	void DeleteElement(GuiElement* element);

public:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<GuiElement*> elements;
	p2List<GuiButton*> command_buttons;
};

#endif