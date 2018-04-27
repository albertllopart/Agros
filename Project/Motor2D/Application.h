#ifndef __APP_H__
#define __APP_H__

#include "p2List.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class Window;
class Render;
class Input;
class Textures;
class Audio;
class Scene;
class Map;
class Player;
class EntityManager;
class Gui;

class Application
{
public:

	// Constructor
	Application(int argc, char* args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	//Get config node
	pugi::xml_node GetConfigNode(const char* objective) const;

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;

	bool LoadGame();
	bool SaveGame();
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	void LoadGameNow();
	void SavegameNow();

public:

	// Modules
	Window* win = nullptr;
	Render* render = nullptr;
	Input* input = nullptr;
	Textures* tex = nullptr;
	Audio* audio = nullptr;
	Scene* scene = nullptr;
	Map* map = nullptr;
	Player* player = nullptr;
	EntityManager* entities = nullptr;
	Gui* gui = nullptr;

private:

	p2List<Module*>	modules;

	int					argc;
	char**				args = nullptr;

	p2SString			title;
	p2SString			organization;

	//XML Related
	pugi::xml_document	config;
	pugi::xml_node		config_node;

	bool		        want_to_save = false;
	bool				want_to_load = false;

	p2SString			load_game;
	mutable p2SString	save_game;

	//timer related
	int					capped_ms;
	float				dt = 0.0f;
	Timer				startup_time;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	PerfTimer			perftimer;
	uint64				frame_count = 0;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	p2SString			cap;
	p2SString			vsync;
};

extern Application* App; // No student is asking me about that ... odd :-S

#endif
