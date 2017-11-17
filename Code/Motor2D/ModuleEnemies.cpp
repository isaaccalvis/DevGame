#include "j1App.h"
#include "ModuleEnemies.h"
#include "EnemyWalker.h"
#include "EnemyBat.h"

ModuleEnemies::ModuleEnemies() {}
ModuleEnemies::~ModuleEnemies() {}

bool ModuleEnemies::Start() {
	bool ret = true;
	
	return ret;
}

bool ModuleEnemies::Update(float dt) {
	bool ret = true;
	updateEnemies();
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
	case E_BAT:
		b = new EnemyBat(x, y);
		break;
	}
	enemies.add(b);
}

void ModuleEnemies::updateEnemies() {
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		if (rec->data->GetIsAlive() == true)
			rec->data->Update();
		else {
			delete rec->data;
			enemies.del(rec);
		}
		rec = rec->next;
	}
}

void ModuleEnemies::receivDamageEnemyAtThisPosition(SDL_Rect rect) {
	p2List_item<BaseEnemy*>* rec = enemies.start;
	while (rec != nullptr) {
		if (rect.x < rec->data->x + rec->data->w &&
			rect.x + rect.w > rec->data->x &&
			rect.y < rec->data->y + rec->data->h &&
			rect.h + rect.y > rec->data->y){
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