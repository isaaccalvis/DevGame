#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "j1Module.h"
#include "Animation.h"
#include "BaseEnemies.h"
#include "p2List.h"

#include "SDL_image\include\SDL_image.h"

enum ENEMY_TYPES {
	E_WALKER,
	E_BAT
};

class BaseEnemy;

class ModuleEnemies : public j1Module
{
public:
	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void addEnemy(ENEMY_TYPES type, float x, float y);

	void updateEnemies(float dt);

	/*bool Load(pugi::xml_node& data);

	bool Save(pugi::xml_node& data) const;*/

	void FindEnemies();

	void receiveDamageEnemyAtThisPosition(SDL_Rect rect);
	void clearEnemies();

private:
	p2List<BaseEnemy*> enemies;
	float dt;

};
#endif