#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "M_Audio.h"
#include "M_Render.h"
#include "M_Window.h"
#include "M_Scene.h"
#include "M_Map.h"
#include "M_Player.h"
#include "M_EntityManager.h"
#include "M_Gui.h"
#include "GuiButton.h"
#include "GuiElement.h"

Scene::Scene() : Module()
{
	name.create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//App->audio->PlayMusic("audio/andy.ogg");

	pugi::xml_node node = App->GetConfigNode("map");

	App->map->LoadMapFromTMX(node.child("level_1").attribute("file").as_string());
	App->map->CreateEntitiesFromTMX();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	App->map->Draw();

	//Save and load
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->LoadGame();
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->SaveGame();

	//tsting
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		App->map->RewriteWalkability(CANI, HIPSTER);
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		App->map->RewriteWalkability(HIPSTER, CANI);


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1000 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1000 * dt;

	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Layers:%d",
		App->map->map_data.width, App->map->map_data.height,
		App->map->map_data.tile_width, App->map->map_data.tile_height,
		App->map->map_data.tilesets.count(), App->map->map_data.layers.count());

	App->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (quit == true)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void Scene::GuiTrigger(GuiElement* element)
{
	GuiButton* button = (GuiButton*)element;

	switch (button->btype)
	{
		case QUIT:
		{
			quit = true;
			break;
		}
	}
}
