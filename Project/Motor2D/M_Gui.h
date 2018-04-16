#ifndef __j1GUI_H__
#define __j1GUI_H__ 

#include "Application.h"
#include "Module.h"
#include "p2List.h"
//#include "j1Fonts.h"

#define CURSOR_WIDTH 2

struct SDL_Rect;
struct SDL_Texture;
enum menu_type;
enum button_type;
enum slider_type;
class GuiElement;
class GuiImage;
class GuiButton;
class GuiSlider;

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

	bool GuiTrigger(GuiElement* element);

	//GuiImage* CreateImage(int x, int y, SDL_Rect rect, menu_type mtype, uint index = 0, bool follows_camera = false);

	//GuiButton* CreateButton(int x, int y, SDL_Rect rect, SDL_Rect mover, SDL_Rect pressed, button_type btype, menu_type mtype, j1Module* callback, bool follows_camera = false);

	//GuiElement* CreateText(int x, int y, char* string, SDL_Color color, _TTF_Font* font, menu_type mtype, bool follows_camera = false);

	//GuiElement* CreateNumber(int x, int y, uint* number, SDL_Color color, _TTF_Font* font, menu_type mtype, bool follows_camera);

	//GuiElement* CreateSlider(int x, int y, SDL_Rect rect, SDL_Rect bar_rect, slider_type stype, menu_type mtype, j1Module* callback, bool follows_camera);

	//void DeleteElement(GuiElement* element);

public:

	SDL_Texture* atlas;
	p2SString atlas_file_name;

	p2List<GuiElement*> elements;
	GuiImage* menubackground = nullptr;

};

#endif