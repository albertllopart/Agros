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
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					texture_width;
	int					texture_height;
	int					tilecount;
	int					columns;
};

struct map_layer
{
	p2SString			name = nullptr;
	int					width = 0;
	int					height = 0;

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
	bool Load(const char* path);

	bool LoadMapFromTMX();

	bool LoadTileset(pugi::xml_node& node);

	bool UnloadMap();

private:


public:

	map_info			map_data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif