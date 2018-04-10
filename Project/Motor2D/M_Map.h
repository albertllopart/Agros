#ifndef __M_MAP_H__
#define __M_MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "Module.h"


struct tileset
{
	int					firstgid;
	p2SString			name;
	int					margin;
	int					spacing = 0;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					texture_width;
	int					texture_height;
	int					tilecount;
	int					columns;

	SDL_Rect GetTileRect(int id) const
	{
		int relative_id = id - firstgid;
		SDL_Rect rect;
		rect.w = tile_width;
		rect.h = tile_height;
		rect.x = margin + ((rect.w + spacing) * (relative_id % tilecount));
		rect.y = margin + ((rect.h + spacing) * (relative_id / tilecount));
		return rect;
	}
};

struct map_layer
{
	p2SString			name = nullptr;
	int					width = 0;
	int					height = 0;
	int					size = 0;

	bool				draw;

	uint*				gid = nullptr;

	uint GetGid(int x, int y)
	{
		return gid[y*width + x];
	}
};

struct map_info
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	p2List<tileset*>	tilesets;
	p2List<map_layer*>	layers;
};

// ----------------------------------------------------
class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool LoadMapFromTMX(const char* path);

	bool LoadMap();

	bool LoadTileset(pugi::xml_node& node);

	bool LoadLayer(pugi::xml_node& node);

	tileset* GetTilesetFromTileId(int id) const;

	//entities
	void CreateEntitiesFromTMX() const;

	bool UnloadMap();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:


public:

	map_info			map_data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif