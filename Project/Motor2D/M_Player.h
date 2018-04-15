#ifndef __M_PLAYER_H__
#define __M_PLAYER_H__

#include	"PugiXml/src/pugixml.hpp"
#include	"Module.h"
#include	"p2List.h"
#include	"p2Point.h"
#include	"M_Textures.h"
#include	"Animation.h"
#include	"Entity.h"
#include	"Unit.h"


#define		TILE_WIDTH	16
#define		PLAYER_WIDTH 22
#define		PLAYER_HIGHT 22

struct SDL_texture;
struct SDL_Rect;

enum player_state
{
	NAVIGATING,
	UNIT_SELECTED,
	BUILDING_SELECTED
};

enum arrow_frame
{
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_VERTICAL,
	ARROW_UP_RIGHT,
	ARROW_UP_LEFT,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_HORIZONTAL,
	ARROW_DOWN_RIGHT,
	ARROW_DOWN_LEFT
};

class Player : public Module
{
public:

	Player();
	~Player();

	bool Start();

	bool CleanUp();

	bool Awake(pugi::xml_node& config);

	bool Update(float dt);

	bool PostUpdate();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

	void Input(float dt);

	void Draw();
	void DrawArrow() const;

private:

	SDL_Texture*		graphic = nullptr;

	iPoint				position;
	iPoint				offset;

	//Animations
	Animation			navigating;
	Animation*			current_animation;

	SDL_Rect			arrow[10];

	//controlling units
	Entity*				selected_unit = nullptr;

public:

	player_state		state;

};
#endif
