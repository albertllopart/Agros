#include "p2Point.h"
#include "Entity.h"
#include "Building.h"
#include "Animation.h"

#define SIZE 16

struct SDL_Texture;
struct SDL_Rect;

class Base : public Building
{
public:

	Base();
	virtual~Base();

	// Called before render is available
	bool				 Awake(pugi::xml_node& config);

	// Called before the first frame
	bool				Start();

	// Called each loop iteration
	bool				Update(float dt);

	// Called before quitting
	bool				CleanUp();

	void				Draw();

	void				ChangeArmy(int new_army);

	iPoint				GetPosition() const;

public:

	iPoint				offset;

};