#ifndef _j1PLAYER_H__
#define _j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

#include "SDL_image\include\SDL_image.h"

enum PLAYER_STATE {
	ANY,
	STAND,
	RUNING,
	JUMPING,
	HABILITY_Q,
	ATAC,
	DEAD
};

enum LOOKING_DIRECTION {
	L_NEUTRAL,
	L_RIGHT,
	L_LEFT
};

struct PlayerData {

	// VARIABLES
	iPoint pos;
	float x, y, w, h;

	float speed = 350.0f;
	float jumpSpeed = 800.0f;
	int contadorAuxiliarAnimacions;							// -1->Stand_R 0->Stand_L, 1->Run_R, 2->Run_L, 3->Jump_R, 4->Jump_L, 5_Dead, 6->TP, 7->Ghost_R, 8->Ghost_L, 9->Caient_R, 10->Caient_L
	uint tempoJump, timeOnAir, tempoTP, tempoDead, tempoAtac;

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
	Animation playerAnimation_DEAD;
	Animation playerAnimation_TP_SMOKE;
	Animation playerAnimation_GHOST_R;
	Animation playerAnimation_GHOST_L;
	Animation playerAnimation_CAIENT_R;
	Animation playerAnimation_CAIENT_L;
	Animation playerAnimation_ATAC_R;
	Animation playerAnimation_ATAC_L;
};

class ModulePlayer : public j1Module
{
public:

	ModulePlayer();
	virtual ~ModulePlayer();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void LoadPLayerTexture();
	void SpawnPLayer();
	void receivDamageByPosition(SDL_Rect rect);

private:

	void DrawPlayer();
	void MovementPlayer();
	void ChargeAnimations();

	void AccioTp();
	void RecolocarPeusPlayer();
	bool AccioMovLaterals(bool col[4]);
	bool AccioMovJump_Gravity(bool col[4]);
	void Atac();

	float dt;

public:


	PlayerData playerData;
	bool col[4] = { false,false,false,false };
};

#endif