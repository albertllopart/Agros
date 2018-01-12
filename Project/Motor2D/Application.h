#ifndef __APP_H__
#define __APP_H__

#include "p2List.h"
#include "Module.h"
//#include "j1PerfTimer.h"
//#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules

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

private:

	p2List<Module*>	modules;

	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	bool		        want_to_save = false;
	bool				want_to_save_audio = false;
	bool                want_to_save_player = false;
	bool				want_to_save_Entity = false;

	bool				want_to_load = false;
	bool				want_to_load_audio = false;
	bool				want_to_load_player = false;
	bool				want_to_load_Entity = false;

	p2SString			load_game;
	mutable p2SString	save_game;

	//timer related
	int					capped_ms;
	float				dt = 0.0f;

	p2SString			cap;
	p2SString			vsync;
};

extern Application* App; // No student is asking me about that ... odd :-S

#endif
