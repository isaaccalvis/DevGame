#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "ModuleEnemies.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "ModulePlayer.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::ChargeColliders() {
	clearCollideRectList();
	App->enemies->clearEnemies();
	p2List_item<MapLayer*>* item = data.layers.start;
	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id == 6) {
					int cont = 0;
					while (cont < map_file.child("map").child("tileset").attribute("tilewidth").as_int()) {
						cont += data.tile_width;
					}
					cont = cont / data.tile_width;
					for (int ii = 0; ii < cont; ii++)
						for (int jj = 0; jj < cont; jj++) {
							iPoint ret(x + ii, y + jj);
							data.colliderOnMap.add(ret);
						}
				}
				else if (tile_id == 10) {
					int cont = 0;
					while (cont < map_file.child("map").child("tileset").attribute("tilewidth").as_int()) {
						cont += data.tile_width;
					}
					cont = cont / data.tile_width;
					for (int ii = 0; ii < cont; ii++)
						for (int jj = 0; jj < cont; jj++) {
							iPoint ret(x + ii, y + jj);
							data.winOnMap = ret;
						}
				}
				else if (tile_id == 1) {
					int cont = 0;
					while (cont < map_file.child("map").child("tileset").attribute("tilewidth").as_int()) {
						cont += data.tile_width;
					}
					cont = cont / data.tile_width;
					for (int ii = 0; ii < cont; ii++)
						for (int jj = 0; jj < cont; jj++) {
							iPoint ret(x + ii, y + jj);
							data.spawnOnMap = ret;
							data.spawnOnMap.x *= data.tilesets.start->data->tile_width;
							data.spawnOnMap.y *= data.tilesets.start->data->tile_height;
						}
				}
				else if (tile_id == 5) {
					int cont = 0;
					while (cont < map_file.child("map").child("tileset").attribute("tilewidth").as_int()) {
						cont += data.tile_width;
					}
					cont = cont / data.tile_width;
					for (int ii = 0; ii < cont; ii++)
						for (int jj = 0; jj < cont; jj++) {
							iPoint ret(x + ii, y + jj);
							data.walkableByEnemyOnMap.add(ret);
						}
				}
			}
		}
	}

	int varAux1 = -1;
	while (varAux1 != 0) {
		varAux1 = 0;
		p2List_item<iPoint> *recAuxBuble = data.colliderOnMap.start;
		if (recAuxBuble != nullptr)
			while (recAuxBuble->next != nullptr) {
				if (recAuxBuble->data.x > recAuxBuble->next->data.x) {
					varAux1++;
					iPoint aux2 = recAuxBuble->data;
					recAuxBuble->data = recAuxBuble->next->data;
					recAuxBuble->next->data = aux2;
				}
					recAuxBuble = recAuxBuble->next;
			}
	}
	CompressColliders();
}

void j1Map::CompressColliders() {
	p2List_item<iPoint> *rec = data.colliderOnMap.start;

	while (rec != nullptr) {
		SDL_Rect aux = { rec->data.x , rec->data.y , 1 , 1 };
		int corrX = 1;
		while (corrX != -1) {
			if (IsCollidingWithTerrainWithoutMapToWORLD(aux.x + corrX, aux.y, RIGHT)) {
				aux.w += 1;
				deleteColliderPoint(aux.x + corrX, aux.y);
				corrX++;
			}
			else {
				corrX = -1;
			}
		}
		data.collidersOnMapRect.add(aux);
		rec = rec->next;
	}

	p2List_item<SDL_Rect> *rec2 = data.collidersOnMapRect.start;
	while (rec2 != nullptr) {
		int i = 1;
		while (i != -1) {
			if (rec2 != nullptr && rec2->next != nullptr) {
				if (rec2->data.x == rec2->next->data.x && rec2->data.y + i == rec2->next->data.y && rec2->data.w == rec2->next->data.w) {
					rec2->data.h++;
					data.collidersOnMapRect.del(rec2->next);
					i++;
				}
				else
					i = -1;
			}
			else
				i = -1;
		}
		rec2 = rec2->next;
	}
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;
	for(; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		//if(layer->properties.Get("Nodraw") != 0)
			//continue;

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x, pos.y, &r, item->data->speed);
				}
			}
		}
	}
	p2List_item<SDL_Rect> *recCOMPT = data.collidersOnMapRect.start;
	int cont2 = 0;
	while (recCOMPT != nullptr) {
		App->render->DrawQuad(SDL_Rect{ recCOMPT->data.x* data.tile_width,recCOMPT->data.y * data.tile_height,recCOMPT->data.w * 128, recCOMPT->data.h * 128 }, 0, 0, 255, 100);
		recCOMPT = recCOMPT->next;
	}

}

//void j1Map::Draw()
//{
//	if (map_loaded == false)
//		return;
//	SDL_Rect* rect;
//	int x, y, h, w;
//	int layerss = 0;
//	for (p2List_item<TileSet*>* blit_tilesets = data.tilesets.start; blit_tilesets != nullptr; blit_tilesets = blit_tilesets->next) {
//		for (p2List_item<MapLayer*>* layer = this->data.layers.start; layer->next != nullptr; layer = layer->next) {
//			layerss++;
//			x = y = h = w = 0;
//
//			for (int id = 0; id < layer->data->size_data; id++) {
//				rect = &blit_tilesets->data->GetTileRect(layer->data->data[id]);
//
//				App->render->Blit(blit_tilesets->data->texture, x, y, rect, layer->data->speed);
//
//				w++;
//				if (w == layer->data->width) {
//					w = 0;
//					h++;
//				}
//
//				x = w * blit_tilesets->data->tile_width;
//				y = h * blit_tilesets->data->tile_height;
//			}
//		}
//	}
//}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while(item)
	{
		if(item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while(item)
	{
		if(id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2) - 1;
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();
	
	data.colliderOnMap.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		if (ret == true)
			ret = LoadLayer(layer, lay);

		data.layers.add(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	ChargeColliders();		// AQUI ES CARREGUEN ELS COLLIDERS


	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}


	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	const char* aux = node.child("properties").child("property").attribute("name").as_string();
	if (strcmp(aux, "Speed") == 0)
	{
		layer->speed = node.child("properties").child("property").attribute("value").as_float();
	}

	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		for (pugi::xml_node iterator = node.child("data").child("tile"); iterator != nullptr; iterator = iterator.next_sibling())
		{
			layer->size_data++;

		}

		layer->data = new uint[layer->size_data];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.add(p);
		}
	}

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	p2List_item<MapLayer*>* item;
	item = data.layers.start;

	for(item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;
				
				if(tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
						map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}

//RETURN_TILE_TYPE j1Map::GetTileByPosition(int x, int y, POSITION_FROM_CENTER posDirection) {
//	iPoint pos = WorldToMap(x, y);
//
//	p2List_item<MapLayer*>* item = data.layers.start;
//	MapLayer* layer = item->data;
//
//	int tile_id = layer->Get(pos.x, pos.y);
//	if (tile_id > 0)
//		printf_s("%i :: x,y-> %i %i\n", tile_id, pos.x, pos.y);
//
//	return RETURN_TILE_TYPE::NORMAL;
//}

bool j1Map::IsCollidingWithTerrainWithoutMapToWORLD(int x, int y, POSITION_FROM_CENTER posCent) { // Aquest es fa servir per compress
	bool ret = false;
	iPoint pos(x, y);
	p2List_item<iPoint>* rec = data.colliderOnMap.start;
	for (; rec != nullptr; rec = rec->next) {
		switch (posCent) {
		case POSITION_FROM_CENTER::DOWN:
			if (rec->data.y >= pos.y)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::UP:
			if (rec->data.y <= pos.y)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::LEFT:
			if (rec->data.x <= pos.x)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::RIGHT:
			if (rec->data.x >= pos.x)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		}
	}
	return ret;
}

bool j1Map::IsCollidingWithTerraint(SDL_Rect rect, POSITION_FROM_CENTER posCent) {
	bool ret = false;
	p2List_item<SDL_Rect>* rec = data.collidersOnMapRect.start;
	switch (posCent) {
	case DOWN:
		while (rec != nullptr) {
			if (rec->data.x * data.tile_width < rect.x + rect.w &&
				rec->data.x * data.tile_width + rec->data.w * data.tile_width > rect.x &&
				rec->data.y * data.tile_height < rect.y + rect.h &&
				rec->data.h * data.tile_height + rec->data.y * data.tile_width > rect.y) {
				ret = true;
				rec = nullptr;
			}
			if (rec != nullptr)
				rec = rec->next;
		}
		break;
	case UP:
		while (rec != nullptr) {
			if (rec->data.x * data.tile_width < rect.x + rect.w &&
				rec->data.x * data.tile_width + rec->data.w * data.tile_width > rect.x &&
				rec->data.y * data.tile_height < rect.y + rect.h &&
				rec->data.h * data.tile_height + rec->data.y * data.tile_width > rect.y) {
				ret = true;
				rec = nullptr;
			}
			if (rec != nullptr)
				rec = rec->next;
		}
		break;
	case RIGHT:
		while (rec != nullptr) {
			if (rec->data.x * data.tile_width < rect.x + rect.w &&
				rec->data.x * data.tile_width + rec->data.w * data.tile_width > rect.x &&
				rec->data.y * data.tile_height < rect.y + rect.h &&
				rec->data.h * data.tile_height + rec->data.y * data.tile_width > rect.y) {
				ret = true;
				rec = nullptr;
			}
			if (rec != nullptr)
				rec = rec->next;
		}
		break;
	case LEFT:
		while (rec != nullptr) {
			if (rec->data.x * data.tile_width < rect.x + rect.w &&
				rec->data.x * data.tile_width + rec->data.w * data.tile_width > rect.x &&
				rec->data.y * data.tile_height < rect.y + rect.h &&
				rec->data.h * data.tile_height + rec->data.y * data.tile_width > rect.y) {
				ret = true;
				rec = nullptr;
			}
			if (rec != nullptr)
				rec = rec->next;
		}
		break;
	}
	return ret;
}

void j1Map::deleteColliderPoint(int x, int y) {
	p2List_item<iPoint>* rec = data.colliderOnMap.start;
	for (; rec != nullptr; rec = rec->next) {
		if (rec->data.x == x && rec->data.y == y) {
			data.colliderOnMap.del(rec);
		}
	}
	delete rec;
}

void j1Map::clearCollideRectList() {
	p2List_item<SDL_Rect>* rec = data.collidersOnMapRect.start;
	while (rec != nullptr) {
		p2List_item<SDL_Rect>* aux = rec;
		rec = rec->next;
		data.collidersOnMapRect.del(aux);
	}
}

bool j1Map::IsCollidingWithWalkableByEnemy(int x, int y, POSITION_FROM_CENTER posCent) {
	bool ret = false;
	iPoint pos = WorldToMap(x, y);
	p2List_item<iPoint>* rec = data.walkableByEnemyOnMap.start;
	for (; rec != nullptr; rec = rec->next) {
		switch (posCent) {
		case POSITION_FROM_CENTER::DOWN:
			if (rec->data.y >= pos.y)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::UP:
			if (rec->data.y <= pos.y)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::LEFT:
			if (rec->data.x <= pos.x)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		case POSITION_FROM_CENTER::RIGHT:
			if (rec->data.x >= pos.x)
				if (rec->data.x == pos.x && rec->data.y == pos.y) {
					ret = true;
					rec = data.colliderOnMap.end;
				}
			break;
		}
	}
	return ret;
}

bool j1Map::IsCollidingWithGoal(int x, int y, POSITION_FROM_CENTER posCent) {
	bool ret = false;
	iPoint pos = WorldToMap(x, y);
	iPoint rec = data.winOnMap;
	switch (posCent) {
	case POSITION_FROM_CENTER::DOWN:
		if (rec.y >= pos.y)
			if (rec.x == pos.x && rec.y == pos.y) {
				ret = true;
			}
		break;
	case POSITION_FROM_CENTER::UP:
		if (rec.y <= pos.y)
			if (rec.x == pos.x && rec.y == pos.y) {
				ret = true;
			}
		break;
	case POSITION_FROM_CENTER::LEFT:
		if (rec.x <= pos.x)
			if (rec.x == pos.x && rec.y == pos.y) {
				ret = true;
			}
		break;
	case POSITION_FROM_CENTER::RIGHT:
		if (rec.x >= pos.x)
			if (rec.x == pos.x && rec.y == pos.y) {
				ret = true;
			}
		break;
	}
	return ret;
}