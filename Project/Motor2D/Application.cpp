#include <iostream> 
#include <sstream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "Application.h"

#include "M_Window.h"
#include "M_Render.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "M_Audio.h"
#include "M_Scene.h"

#include "Brofiler\Brofiler.h"

// Constructor
Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	win = new Window();
	render = new Render();
	input = new Input();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();

	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);

	cap = "ON";
	vsync = "ON";
}

// Destructor
Application::~Application()
{
	// release modules
	p2List_item<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void Application::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool Application::Awake()
{
	pugi::xml_parse_result result = config.load_file("config.xml");
	pugi::xml_node app_config;

	bool ret = false;
	if (result.status == 0)//0 means ok, 1 equals to failure (missing file)
		ret = true;

	config_node = LoadConfig(config);

	if (config_node.empty() == false)
	{
		// self-config
		app_config = config_node.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(30);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if (ret == true)
	{
		p2List_item<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->Awake(config_node.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool Application::Start()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool Application::Update()
{
		/*if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			if (capped_ms == -1000)
			{
				capped_ms = 1000 / 30;
				cap = "ON";
			}
			else
			{
				capped_ms = -1000;
				cap = "OFF";
			}
		}*/

	bool ret = true;
	PrepareUpdate();

	/*if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;*/

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node Application::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	/*frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();*/
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	/*if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "FPS: %i Av.FPS: %.2f Last Frame Ms: %u Cap: %s Vsync: %s",
		frames_on_last_update, avg_fps, last_frame_ms, cap.GetString(), vsync.GetString());

	if (App->scene->title_2 == false)
		App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}*/
}

// Call modules before each loop iteration
bool Application::PreUpdate()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool Application::DoUpdate()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool Application::PostUpdate()
{
	bool ret = true;
	p2List_item<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int Application::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Application::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* Application::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* Application::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
bool Application::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	return true;
}

// ---------------------------------------
bool Application::SaveGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	return true;
}




// ---------------------------------------
void Application::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

void Application::LoadGameNow()
{
	pugi::xml_document document;
	pugi::xml_node root_node;

	pugi::xml_parse_result result = document.load_file("save_game.xml");

	root_node = document.child("save");

	for (int i = 0; i < modules.count(); i++)
	{
		modules[i]->Load(root_node.child(modules[i]->name.GetString()));
	}

	want_to_load = false;
}

void Application::SavegameNow()
{
	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document document;
	pugi::xml_node root_node;


	root_node = document.append_child("save");

	for (int i = 0; i < modules.count(); i++)
	{
		pugi::xml_node save_node;
		save_node = root_node.append_child(modules[i]->name.GetString());
		modules[i]->Save(save_node);
	}

	document.save_file("save_game.xml");
	want_to_save = false;
}

float Application::GetDT() const
{
	return dt;
}
