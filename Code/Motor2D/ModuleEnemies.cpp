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

void ModuleEnemies::addEnemy(ENEMY_TYPES type , float x, float y) {
	BaseEnemy* b;
	switch (type) {
	case E_WALKER:
		b = new EnemyWalker(x, y);
		break;
	case E_FLYER:
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
			addEnemy(ENEMY_TYPES::E_FLYER, ret.x, ret.y);
		}
	}
}