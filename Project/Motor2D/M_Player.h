#ifndef __M_PLAYER_H__
#define __M_PLAYER_H__

#include	"PugiXml/src/pugixml.hpp"
#include	"Module.h"
#include	"p2List.h"
#include	"p2Point.h"
#include	"M_Textures.h"
#include	"Animation.h"

#define		TILE_WIDTH	16
#define		PLAYER_WIDTH 22
#define		PLAYER_HIGHT 22
#define		OFFSET -3

struct SDL_texture;
struct SDL_Rect;

enum player_state
{
	UNKNOWN = 0,
	IDLE
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

private:

	SDL_Texture*		graphic = nullptr;

	iPoint				position;
	player_state		state;

	//Animations
	Animation			idle;
	Animation*			current_animation;

};
#endif
