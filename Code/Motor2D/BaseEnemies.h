#ifndef __BaseEnemy_H__
#define __BaseEnemy_H__

#include "SDL_image\include\SDL_image.h"
#include "ModulePlayer.h"

class BaseEnemy {
public:
	BaseEnemy() {}
	~BaseEnemy() {}

	virtual void Update() = 0;
	
	bool GetIsAlive() { return isAlive; }
	float x, y, w, h;
protected:
	SDL_Texture* texturaEnemy;
	Animation enemyAnim;
	bool isAlive = true;
};

#endif