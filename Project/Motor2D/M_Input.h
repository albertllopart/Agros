#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"
#include "SDL\include\SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define NUM_CONTROLLER_BUTTONS 8
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum input_state
{
	UI_INPUT,
	PLAYER_INPUT
};

enum j1EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum j1KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{
public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(j1EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	j1KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	j1KeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	j1KeyState GetControllerButton(int id) const
	{
		return controller_buttons[id];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

private:

	bool		windowEvents[WE_COUNT];
	j1KeyState*	keyboard;
	j1KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;

	//joystick
	SDL_GameController*		controller = nullptr;
	SDL_Joystick*			joystick = nullptr;
	j1KeyState				controller_buttons[NUM_CONTROLLER_BUTTONS];

public:

	input_state state;

};

#endif // __M_INPUT_H__