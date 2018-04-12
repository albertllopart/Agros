#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Render.h"
#include "M_Textures.h"
#include "M_Map.h"
#include <math.h>
#include "M_EntityManager.h"

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

	p2List_item<map_layer*>* layer = map_data.layers.start;

	while (layer != NULL)
	{
		if (layer->data->draw == true)
		{
			for (uint x = 0; x < layer->data->width; x++)
			{
				for (uint y = 0; y < layer->data->height; y++)
				{
					int id = layer->data->GetGid(x, y);
					iPoint position = MapToWorld(x, y);
					tileset* set = GetTilesetFromTileId(id);
					SDL_Rect rect = set->GetTileRect(id);
					App->render->Blit(set->texture, position.x, position.y, &rect);
				}
			}
		}
		layer = layer->next;
	}
		
}

//BFS
void Map::ResetBFS(iPoint position)
{
	frontier.Clear();
	visited.clear();
	frontier.Push(position);
	visited.add(position);
}

void Map::PropagateBFS()
{
	
	while (frontier.start != NULL)
	{
		iPoint popped = frontier.start->data;
		frontier.Pop(popped);

		if (IsWalkable(popped.x, popped.y))
		{
			iPoint north(popped.x, popped.y - 1);
			iPoint south(popped.x, popped.y + 1);
			iPoint east(popped.x + 1, popped.y);
			iPoint west(popped.x - 1, popped.y);

			if (visited.find(north) == -1 && IsWalkable(north.x, north.y))
			{
				frontier.Push(north);
				visited.add(north);
			}
			if (visited.find(south) == -1 && IsWalkable(south.x, south.y))
			{
				frontier.Push(south);
				visited.add(south);
			}
			if (visited.find(east) == -1 && IsWalkable(east.x, east.y))
			{
				frontier.Push(east);
				visited.add(east);
			}
			if (visited.find(west) == -1 && IsWalkable(west.x, west.y))
			{
				frontier.Push(west);
				visited.add(west);
			}
		}
	}
}

void Map::DrawBFS()
{
	iPoint point;

	// Draw visited
	p2List_item<iPoint>* item = visited.start;

	while (item)
	{
		point = item->data;
		tileset* tileset = GetTilesetFromTileId(137);

		SDL_Rect r = tileset->GetTileRect(137);
		iPoint pos = MapToWorld(point.x, point.y);

		App->render->Blit(tileset->texture, pos.x, pos.y, &r);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));
		tileset* tileset = GetTilesetFromTileId(138);

		SDL_Rect r = tileset->GetTileRect(138);
		iPoint pos = MapToWorld(point.x, point.y);

		App->render->Blit(tileset->texture, pos.x, pos.y, &r);
	}

}

bool Map::IsWalkable(int x, int y) const
{
	// TODO 3: return true only if x and y are within map limits
	// and the tile is walkable (tile id 138 in the navigation layer)
	p2List_item<map_layer*>* walkability = map_data.layers.start;
	
	while (walkability->data->name != "walkability" && walkability != NULL)
	{
		walkability = walkability->next;
	}

	if (walkability != NULL)
	{
		if (walkability->data->GetGid(x, y) == 138 || x > map_data.width || x < 0 || y > map_data.height || y < 0)
			return false;
	}

	return true;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	UnloadMap();

	map_file.reset();

	return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * map_data.tile_width;
	ret.y = y * map_data.tile_height;

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / map_data.tile_width;
	ret.y = y / map_data.tile_height;

	return ret;
}

// Load new map
bool Map::LoadMapFromTMX(const char* file_name)
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
		LoadMap();
	}

	pugi::xml_node node = map_file.child("map");
	pugi::xml_node iterator = node.first_child();
	p2SString name = iterator.name();

	while (iterator != NULL)
	{
		if (name == "tileset")
		{ 
			LoadTileset(iterator);
		}
		iterator = iterator.next_sibling();
		name = iterator.name();
	}

	node = map_file.child("map");
	iterator = node.child("layer");
	name = iterator.name();

	while (iterator != NULL)
	{
		if (name == "layer")
		{
			LoadLayer(iterator);
		}
		iterator = iterator.next_sibling();
		name = iterator.name();
	}


	if (ret == true)
	{
		LOG("Loaded Map: %s", file_name);

		p2List_item<tileset*>* tileset_item = map_data.tilesets.start;

		while (tileset_item != NULL)
		{
			LOG("Loaded Tileset: %s", tileset_item->data->name.GetString());
			tileset_item = tileset_item->next;
		}

		p2List_item<map_layer*>* layer_item = map_data.layers.start;

		while (layer_item != NULL)
		{
			LOG("Loaded Layer: %s", layer_item->data->name.GetString());
			layer_item = layer_item->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool Map::LoadMap()
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

bool Map::LoadLayer(pugi::xml_node& node)
{
	bool ret = true;

	if (node != NULL)
	{
		map_layer* yer = new map_layer();

		yer->name.create(node.attribute("name").as_string());
		yer->width = node.attribute("width").as_int();
		yer->height = node.attribute("height").as_int();
		yer->size = yer->width * yer->height;

		//to draw or not to draw
		pugi::xml_node draw = node.child("properties");
		
		if (draw.child("property").attribute("value").as_int() == 1)
		{
			yer->draw = true;
		}
		else
		{
			yer->draw = false;
		}

		yer->gid = new uint[yer->size];
		memset(yer->gid, 0, yer->size);

		int i = 0;

		for (pugi::xml_node iterator = node.child("data").child("tile"); iterator; iterator = iterator.next_sibling("tile"))
		{
			yer->gid[i] = iterator.attribute("gid").as_uint();
			i++;
		}

		map_data.layers.add(yer);
	}

	return ret;
}

tileset* Map::GetTilesetFromTileId(int id) const
{
	p2List_item<tileset*>* endset = map_data.tilesets.end;

	for (p2List_item<tileset*>* set = map_data.tilesets.start; set != NULL; set = set->next)
	{
		if (set->next != NULL && id >= set->data->firstgid && id < set->next->data->firstgid)
		{
			return set->data;
		}
	}
	return endset->data;
}

void Map::CreateEntitiesFromTMX() const
{
	p2List_item<map_layer*>* item = nullptr;

	for (p2List_item<map_layer*>* iterator = map_data.layers.start; iterator != NULL; iterator = iterator->next)
	{
		if (iterator->data->name == "buildings")
		{
			item = iterator;
		}
	}

	for (int x = 0; x < map_data.width; x++)
	{
		for (int y = 0; y < map_data.height; y++)
		{
			uint current_gid = item->data->GetGid(x, y);

			if (current_gid == 37)//canibase
			{
				iPoint pos(x, y);
				App->entities->CreateCanibase(pos);
			}
			else if (current_gid == 38)//factory
			{
				iPoint pos(x, y);
				App->entities->CreateFactory(pos);
			}
		}
	}
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
