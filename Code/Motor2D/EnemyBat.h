#ifndef __EnemyBat_H__
#define __EnemyBat_H__

#include "BaseEnemies.h"
#include "SDL_image\include\SDL_image.h"
#include "Animation.h"

#define MOVE_VELOCITY 5.0f
#define DISTANCIA_MIN_ATAC 30.0f

enum ENEMY_BAT_STATES {
	EF_STILL,
	EF_FLY,
	EF_DEAD,
	EF_ATAC
};

class EnemyBat : public BaseEnemy {
public:

	EnemyBat(float x, float y);
	~EnemyBat();

	void Update(float dt);

	void ChargeAnimations();
	void Draw();
	void UpdateInfo();
	void Move(LOOKING_DIRECTION direction, float dt);

	p2DynArray<iPoint>* lastpath;

	iPoint nextpos;

private:

	ENEMY_BAT_STATES enemyBatState;
	LOOKING_DIRECTION enemyBatLookingDirection;

	Animation Still;
	Animation Fly_R;
	Animation Fly_L;
	Animation Dead_R;
	Animation Dead_L;
	Animation Explosion;

	int controladorAnimations;	// 0->Still, 1->FlyL, 2->FlyR, 3->DeadR, 4->DeadL, 5->Explosion
	bool canAtacPlayer;

};

#endif