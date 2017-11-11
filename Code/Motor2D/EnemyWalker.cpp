#include "EnemyWalker.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"

EnemyWalker::EnemyWalker(float x, float y) {
	this->x = x;
	this->y = y;
	w = 50;
	h = 65;
	texturaEnemy = App->tex->Load("textures/Enemy_Walker_1.png");
	ChargeAnimations();
	enemyWalkerLookingDirection = L_LEFT;
	enemyWalkerState = E_STAND;
	controladorAnimations = 0;
}

EnemyWalker::~EnemyWalker() {}

void EnemyWalker::Update() {
	Draw();
}

void EnemyWalker::ChargeAnimations() {
	Stand_R.PushBack({ 1,0,50,65 });
	Stand_R.PushBack({ 56,0,50,65 });
	Stand_R.speed = 0.08f;

	Stand_L.PushBack({ 387,0,50,65 });
	Stand_L.PushBack({ 436,0,50,65 });
	Stand_L.speed = 0.08f;

	Walk_R.PushBack({ 0,70,42,65 });
	Walk_R.PushBack({ 54,70,42,65 });
	Walk_R.PushBack({ 102,70,42,65 });
	Walk_R.PushBack({ 154,70,42,65 });
	Walk_R.speed = 0.25f;

	Walk_L.PushBack({ 290,70,42,65 });
	Walk_L.PushBack({ 340,70,42,65 });
	Walk_L.PushBack({ 391,70,42,65 });
	Walk_L.PushBack({ 444,70,42,65 });
	Walk_L.speed = 0.25f;

	Jump_R.PushBack({ 1,269, 50 , 64 });
	Jump_R.PushBack({ 58,269, 50 , 64 });
	Jump_R.PushBack({ 112,269, 50 , 64 });
	Jump_R.PushBack({ 164,269, 50 , 64 });
	Jump_R.loop = false;
	Jump_R.speed = 0.25f;

	Jump_L.PushBack({ 274,269, 50 , 64 });
	Jump_L.PushBack({ 329,269, 50 , 64 });
	Jump_L.PushBack({ 382,269, 50 , 64 });
	Jump_L.PushBack({ 436,269, 50 , 64 });
	Jump_L.loop = false;
	Jump_L.speed = 0.25f;

	Atac_R;
	Atac_L;
	Dead_R;
	Dead_L;
}

void EnemyWalker::Draw() {
	if (texturaEnemy != nullptr) {
		if (enemyWalkerLookingDirection == L_RIGHT)
			switch (enemyWalkerState) {
			case E_STAND:
				if (controladorAnimations != 0) {
					enemyAnim = Stand_R;
					controladorAnimations = 0;
				}
				break;
			case E_WALK:
				if (controladorAnimations != 2) {
					enemyAnim = Walk_R;
					controladorAnimations = 2;
				}
				break;
			case E_JUMP:
				if (controladorAnimations != 4) {
					enemyAnim = Jump_R;
					controladorAnimations = 4;
				}
				break;
			case E_ATAC:
				if (controladorAnimations != 6) {
					enemyAnim = Atac_R;
					controladorAnimations = 6;
				}
				break;
			case E_DEAD:
				if (controladorAnimations != 8) {
					enemyAnim = Dead_R;
					controladorAnimations = 8;
				}
				break;
			}
		else
			switch (enemyWalkerState) {
			case E_STAND:
				if (controladorAnimations != 1) {
					enemyAnim = Stand_L;
					controladorAnimations = 1;
				}
				break;
			case E_WALK:
				if (controladorAnimations != 3) {
					enemyAnim = Walk_L;
					controladorAnimations = 3;
				}
				break;
			case E_JUMP:
				if (controladorAnimations != 5) {
					enemyAnim = Jump_L;
					controladorAnimations = 5;
				}
				break;
			case E_ATAC:
				if (controladorAnimations != 7) {
					enemyAnim = Atac_L;
					controladorAnimations = 7;
				}
				break;
			case E_DEAD:
				if (controladorAnimations != 9) {
					enemyAnim = Dead_L;
					controladorAnimations = 9;
				}
				break;
			}
		App->render->Blit(texturaEnemy, x, y, &enemyAnim.GetCurrentFrame());
	}
}

void EnemyWalker::MoveDirection(LOOKING_DIRECTION direction) {
	switch (direction) {
	case L_RIGHT:

		break;
	case L_LEFT:

		break;
	default:

		break;
	}
}