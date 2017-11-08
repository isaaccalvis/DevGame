#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"

#include "SDL_image\include\SDL_image.h"

enum PLAYER_STATE {
	ANY,
	STAND,
	RUNING,
	JUMPING,
	HABILITY_Q,
	DEAD
};

enum LOOKING_DIRECTION {
	L_RIGHT,
	L_LEFT
};

struct PlayerData {

	// VARIABLES
	float x, y, w, h, xFum, yFum;
	int contadorAuxiliarAnimacions;							// -1->Stand_R 0->Stand_L, 1->Run_R, 2->Run_L, 3->Jump_R, 4->Jump_L, 5_Dead, 6->TP
	uint tempoJump, timeOnAir, tempoPerTP;
	SDL_Texture* playerSprites;

	// CONTROL POSICIO I SITUACIO
	PLAYER_STATE playerState;
	LOOKING_DIRECTION lookingWay;

	// ANIMATIONS
	Animation playerAnim;
	Animation playerAnimation_STAND_R;
	Animation playerAnimation_STAND_L;
	Animation playerAnimation_RUN_R;
	Animation playerAnimation_RUN_L;
	Animation playerAnimation_JUMP_R;
	Animation playerAnimation_JUMP_L;
	Animation playerAnimation_TP_SMOKE;
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
	void AccioTp();
	bool AccioMovLaterals(bool col[4]);
	bool AccioMovJump_Gravity(bool col[4]);
public:
	PlayerData playerData;
	
};

#endif