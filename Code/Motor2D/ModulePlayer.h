#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"

#include "SDL_image\include\SDL_image.h"

enum PLAYER_STATE {
	ANY,
	STAND,
	RUNING,
	JUMPING,
	DEAD
};

struct PlayerData {
	float x, y;
	uint tempoJump, timeOnAir;
	SDL_Texture* playerSprites;
	SDL_Rect playerRect;
	PLAYER_STATE playerState;
};

class ModulePlayer : public j1Module
{
public:

	ModulePlayer();
	virtual ~ModulePlayer();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void DrawPlayer();
	void MovimentPlayer();
private:

public:
	PlayerData playerData;
	
};

#endif