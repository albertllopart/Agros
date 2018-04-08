#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Map.h"
#include <math.h>

Map::Map() : Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void Map::Draw()
{
	if (map_loaded == false)
		return;

	p2List_item<tileset*>* item = map_data.tilesets.start;
	while (item != NULL)
	{
		App->render->Blit(item->data->texture, 0, 0);
		item = item->next;
	}

}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	UnloadMap();

	map_file.reset();

	return true;
}

// Load new map
bool Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if (ret == true)
	{
		LoadMapFromTMX();
	}

	pugi::xml_node node = map_file.child("map");
	node = node.child("tileset");

	while (node != NULL)
	{
		LoadTileset(node);
		node = node.next_sibling();
	}


	if (ret == true)
	{
		LOG("Loaded Map: %s", file_name);

		p2List_item<tileset*>* item = map_data.tilesets.start;

		while (item != NULL)
		{
			LOG("Loaded Tileset: %s", item->data->name.GetString());
			item = item->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool Map::LoadMapFromTMX()
{
	bool ret = true;

	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{ 
		map_data.width = map.attribute("width").as_int();
		map_data.height = map.attribute("height").as_int();
		map_data.tile_width = map.attribute("tilewidth").as_int();
		map_data.tile_height = map.attribute("tileheight").as_int();
	}

	return ret;
}

bool Map::LoadTileset(pugi::xml_node& node)
{
	bool ret = true;

	if (node != NULL)
	{
		tileset* set = new tileset();

		set->firstgid = node.attribute("firstgid").as_int();
		set->name.create(node.attribute("name").as_string());
		set->margin = node.attribute("margin").as_int();
		set->spacing = node.attribute("spacing").as_int();
		set->tile_width = node.attribute("tilewidth").as_int();
		set->tile_height = node.attribute("tileheight").as_int();
		
		pugi::xml_node image = node.child("image");

		if (image != NULL)
		{
			set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));

			int w, h;
			SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
			set->texture_width = image.attribute("width").as_int();

			if (set->texture_width <= 0)
			{
				set->texture_width = w;
			}

			set->texture_height = image.attribute("height").as_int();

			if (set->texture_height <= 0)
			{
				set->texture_height = h;
			}

			set->tilecount = set->texture_width / set->tile_width;
			set->columns = set->texture_height / set->tile_height;
		}

		map_data.tilesets.add(set);
	}

	return ret;
}

bool Map::UnloadMap()
{
	p2List_item<tileset*>* tileset_item = map_data.tilesets.start;
	while (tileset_item != NULL)
	{
		SDL_DestroyTexture(tileset_item->data->texture);	
		tileset_item = tileset_item->next;
	}

	p2List_item<map_layer*>* layer_item = map_data.layers.start;
	while (layer_item != NULL)
	{
		delete layer_item->data->gid;
		layer_item = layer_item->next;
	}

	map_data.tilesets.clear();
	map_data.layers.clear();

	return true;
}
