#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "Animation.h"
#include "BaseEnemies.h"

#include "SDL_image\include\SDL_image.h"

enum ENEMY_TYPES{
	E_WALKER,
	E_BAT
};

class ModuleEnemies : public j1Module
{
public:
	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void addEnemy(ENEMY_TYPES type, float x, float y);
	void updateEnemies();
	void receivDamageEnemyAtThisPosition(SDL_Rect rect);
	void clearEnemies();
private:
	p2List<BaseEnemy*> enemies;

};
#endif