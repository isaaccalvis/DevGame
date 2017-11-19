#include "j1App.h"
#include "ModuleEnemies.h"
#include "EnemyWalker.h"
#include "EnemyBat.h"

#include "j1Map.h"

ModuleEnemies::ModuleEnemies() {}
ModuleEnemies::~ModuleEnemies() {}

bool ModuleEnemies::Start() {
	bool ret = true;
	
	FindEnemies();

	return ret;
}

bool ModuleEnemies::Update(float dt) {
	bool ret = true;
	updateEnemies(dt);
	return ret;
}

bool ModuleEnemies::CleanUp() {
	bool ret = true;
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		delete rec->data;
		enemies.del(rec);
		rec = rec->next;
	}
	return ret;
}

//bool ModuleEnemies::Load(pugi::xml_node& data)
//{
//	x = data.child("camera").attribute("x").as_int();
//	y = data.child("camera").attribute("y").as_int();
//
//	return true;
//}
//
//// Save Game State
//bool ModuleEnemies::Save(pugi::xml_node& data) const
//{
//	pugi::xml_node cam = data.append_child("camera");
//
//	cam.append_attribute("x") = camera.x;
//	cam.append_attribute("y") = camera.y;
//
//	return true;
//}

void ModuleEnemies::addEnemy(ENEMY_TYPES type , float x, float y) {
	BaseEnemy* b;
	switch (type) {
	case E_WALKER:
		b = new EnemyWalker(x, y);
		break;

	case E_BAT:
		b = new EnemyBat(x, y);
		break;
	}
	enemies.add(b);
}

void ModuleEnemies::updateEnemies(float dt) {
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		if (rec->data->GetIsAlive() == true)
			rec->data->Update(dt);
		else {
			delete rec->data;
			enemies.del(rec);
		}
		rec = rec->next;
	}
}


void ModuleEnemies::FindEnemies() {
	p2List_item<MapLayer*>* item = App->map->data.layers.end;

	for (int i = 0; i < item->data->size_data; i++)
	{
		if (item->data->data[i] == 23)
		{
			iPoint ret = App->map->TiletoWorld(i);
			addEnemy(ENEMY_TYPES::E_BAT, ret.x, ret.y);
		}

		if (item->data->data[i] == 22)
		{
			iPoint ret = App->map->TiletoWorld(i);
			addEnemy(ENEMY_TYPES::E_WALKER, ret.x, ret.y);
		}
	}
}

void ModuleEnemies::receiveDamageEnemyAtThisPosition(SDL_Rect rect) {
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		if (rect.x < rec->data->pos.x + rec->data->w &&
			rect.x + rect.w > rec->data->pos.x &&
			rect.y < rec->data->pos.y + rec->data->h &&
			rect.h + rect.y > rec->data->pos.y){
			rec->data->~BaseEnemy();
			enemies.del(rec);
		}
		rec = rec->next;
	}
}

void ModuleEnemies::clearEnemies() {
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		p2List_item<BaseEnemy*>* aux = rec;
		rec = rec->next;
		enemies.del(aux);
	}
}