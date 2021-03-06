#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

enum POSITION_FROM_CENTER {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum MAP_OBJECTS_TYPES {
	TERRAIN_COLLIDER,
	WALKABLE_BY_ENEMY
};

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while(item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

struct MapLayer
{
	MapLayer() : data(NULL) {}

	~MapLayer() {
		RELEASE(data);
	}

	p2SString	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;
	float		speed = 1;
	uint		size_data = 0;

	inline uint Get(int x, int y) const {
		return data[(y*width) + x];
	}
};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	SDL_Texture*		texture;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<iPoint> colliderOnMap;
	p2List<SDL_Rect> collidersOnMapRect;
	p2List<iPoint> walkableByEnemyOnMap;

	iPoint winOnMap;
	iPoint spawnOnMap;
};

class j1Map : public j1Module
{
public:

	j1Map();
	virtual ~j1Map();

	bool Awake(pugi::xml_node& conf);
	void Draw();
	bool CleanUp();
	bool Load(const char* path);	// LOAD NEW MAP

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	iPoint TiletoWorld(int i) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void ChargeColliders();
	void CompressColliders();
	bool ac = false;
	bool IsCollidingWithWalkableByEnemy(int x, int y, POSITION_FROM_CENTER posCent);
	bool IsCollidingWithTerrainWithoutMapToWORLD(int x, int y, POSITION_FROM_CENTER posCent);
	bool IsCollidingWithTerraint(SDL_Rect rect, POSITION_FROM_CENTER posCent);
	void deleteColliderPoint(int x, int y);
	void clearCollideRectList();
	bool IsCollidingWithGoal(int x, int y, POSITION_FROM_CENTER posCent);

	bool logic_draw = false;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif