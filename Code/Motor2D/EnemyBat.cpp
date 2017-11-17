#include "EnemyBat.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"

EnemyBat::EnemyBat(float x, float y) {
	this->x = x;
	this->y = y;
	w = 50;
	h = 65;
	texturaEnemy = App->tex->Load("textures/Enemy_Air_1.png");
	ChargeAnimations();
	enemyBatLookingDirection = L_NEUTRAL;
	enemyBatState = EF_STILL;
	controladorAnimations = -1;
	canAtacPlayer = false;
}

EnemyBat::~EnemyBat() {}

void EnemyBat::Update() {
	UpdateInfo();
	Move(enemyBatLookingDirection);
	Draw();
}

void EnemyBat::UpdateInfo() {
	if (App->player->playerData.x - this->x > (-3 * App->map->data.tile_width)&& App->player->playerData.x - this->x < (3 * App->map->data.tile_width))
		canAtacPlayer = true;
	else
		canAtacPlayer = false;
	if (!canAtacPlayer)
		enemyBatLookingDirection = L_NEUTRAL;
	else {
		// MIRAR CAP AL PLAYER
		if (App->player->playerData.x > x)
			enemyBatLookingDirection = L_RIGHT;
		else
			enemyBatLookingDirection = L_LEFT;
		// MIRAR SI TOCA AL PLAYER
		if (enemyBatLookingDirection == L_RIGHT) {
			if (App->player->playerData.x - x < DISTANCIA_MIN_ATAC)
				enemyBatState = EF_EXPLOSION;
			else
				enemyBatState = EF_FLY;
		}
		else {
			if (x - App->player->playerData.x < DISTANCIA_MIN_ATAC)
				enemyBatState = EF_EXPLOSION;
			else
				enemyBatState = EF_FLY;
		}
	}
}
// TXELL SEXY
void EnemyBat::Move(LOOKING_DIRECTION direction) {
	if (enemyBatLookingDirection == L_RIGHT) {
		switch (enemyBatState) {
		case EF_EXPLOSION:

			break;
		case EF_FLY:

			break;
		case EF_DEAD:

			break;
		}
	}
	else {
		switch (enemyBatState) {
		case EF_EXPLOSION:

			break;
		case EF_FLY:

			break;
		case EF_DEAD:

			break;
		}
	}
}


void EnemyBat::ChargeAnimations() {

	Still.PushBack({ 33, 1, 31, 26 });
	Still.PushBack({ 65, 1, 31, 26 });
	Still.PushBack({ 97, 1, 31, 26 });
	Still.speed = 0.08f;

	Fly_R.PushBack({ 41, 33, 17, 26 });
	Fly_R.PushBack({ 73, 33, 17, 26 });
	Fly_R.PushBack({ 106, 33, 17, 26 });
	Fly_R.speed = 0.08f;

	Fly_L.PushBack({ 38, 97, 17, 26 });
	Fly_L.PushBack({ 70, 97, 17, 26 });
	Fly_L.PushBack({ 101, 97, 17, 26 });
	Fly_L.speed = 0.08f;

	Dead_R.PushBack({ 7, 49, 19, 15 });

	Dead_L.PushBack({ 6, 113, 19, 15 });

	Explosion;

}

void EnemyBat::Draw() {
	if (texturaEnemy != nullptr) {
		if (enemyBatLookingDirection == L_NEUTRAL) {
			if (controladorAnimations != 0) {
				enemyAnim = Still;
				controladorAnimations = 0;
			}
		}
			
		else if (enemyBatLookingDirection == L_RIGHT)
			switch (enemyBatState) {
			case EF_FLY:
				if (controladorAnimations != 2) {
					enemyAnim = Fly_R;
					controladorAnimations = 2;
				}
				break;
			case EF_DEAD:
				if (controladorAnimations != 3) {
					enemyAnim = Dead_R;
					controladorAnimations = 3;
				}
				break;
			}
		else
			switch (enemyBatState) {
			case EF_FLY:
				if (controladorAnimations != 1) {
					enemyAnim = Fly_L;
					controladorAnimations = 1;
				}
				break;
			case EF_DEAD:
				if (controladorAnimations != 4) {
					enemyAnim = Dead_L;
					controladorAnimations = 4;
				}
				break;
			}
		App->render->Blit(texturaEnemy, x, y, &enemyAnim.GetCurrentFrame());
	}
}