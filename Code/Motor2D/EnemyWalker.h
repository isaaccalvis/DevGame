#ifndef __EnemyWalker_H__
#define __EnemyWalker_H__

#include "BaseEnemies.h"
#include "SDL_image\include\SDL_image.h"
#include "Animation.h"

#define MOVE_VELOCITY 5.0f
#define DISTANCIA_MIN_ATAC 30.0f

enum ENEMY_WALKER_STATES {
	E_STAND,
	E_WALK,
	E_JUMP,
	E_ATAC,
	E_DEAD,
	E_SMOKE
};

class EnemyWalker : public BaseEnemy {
public:

	EnemyWalker(float x, float y);
	~EnemyWalker();

	void Update();

	void ChargeAnimations();
	void Draw();
	void UpdateInfo();
	void Move(LOOKING_DIRECTION direction);
	void Jump(LOOKING_DIRECTION direction);
	void gravityFall();
private:

	ENEMY_WALKER_STATES enemyWalkerState;
	LOOKING_DIRECTION enemyWalkerLookingDirection;

	Animation Stand_R;
	Animation Stand_L;
	Animation Walk_R;
	Animation Walk_L;
	Animation Jump_R;
	Animation Jump_L;
	Animation Atac_R;
	Animation Atac_L;
	Animation Dead_R;
	Animation Dead_L;
	Animation Smoke;

	int controladorAnimations;	// 0->StandR, 1->StandL, 2->WalkR, 3->WalkL, 4->JumpR, 5->JumpL, 6->AtacR, 7->AtacL, 8->DeadR, 9->DeadL, 10->smoke
	uint tempoSmokeJump, tempoAtac;
	bool canAtacPlayer;
	bool col[4];
	bool walkable[4];
};

#endif