#ifndef __BaseEnemy_H__
#define __BaseEnemy_H__

#include "SDL_image\include\SDL_image.h"
#include "p2Point.h"
#include "ModulePlayer.h"
#include "j1Pathfinding.h"

class BaseEnemy {
public:
	BaseEnemy() {}
	~BaseEnemy() {}

	virtual void Update(float dt) = 0;
	
	bool GetIsAlive() { return isAlive; }
	float w, h;

	fPoint pos;
	float dt;
protected:
	SDL_Texture* texturaEnemy;
	Animation enemyAnim;
	bool isAlive = true;
};

#endif