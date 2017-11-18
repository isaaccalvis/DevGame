#include "EnemyBat.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"

EnemyBat::EnemyBat(float x, float y) {
	pos.x = x;
	pos.y = y;
	w = 50;
	h = 65;
	texturaEnemy = App->tex->Load("textures/Enemy_Air_1.png");
	ChargeAnimations();
	enemyAnim = Still;
	enemyBatLookingDirection = L_NEUTRAL;
	enemyBatState = EF_STILL;
	controladorAnimations = -1;
	canAtacPlayer = false;
}

EnemyBat::~EnemyBat() {}

void EnemyBat::Update(float dt) {
	UpdateInfo();
	Move(enemyBatLookingDirection, dt);
	Draw();
}

void EnemyBat::UpdateInfo() {
	if (App->player->playerData.x - this->pos.x > (-3 * App->map->data.tile_width)&& App->player->playerData.x - this->pos.x < (3 * App->map->data.tile_width))
		canAtacPlayer = true;
	else
		canAtacPlayer = false;
	if (!canAtacPlayer)
		enemyBatLookingDirection = L_NEUTRAL;
	else {
		// MIRAR CAP AL PLAYER
		if (App->player->playerData.x > pos.x)
			enemyBatLookingDirection = L_RIGHT;
		else
			enemyBatLookingDirection = L_LEFT;
		// MIRAR SI TOCA AL PLAYER
		if (enemyBatLookingDirection == L_RIGHT) {
			if (App->player->playerData.x - pos.x < DISTANCIA_MIN_ATAC)
				enemyBatState = EF_EXPLOSION;
			else
				enemyBatState = EF_FLY;
		}
		else {
			if (pos.x - App->player->playerData.x < DISTANCIA_MIN_ATAC)
				enemyBatState = EF_EXPLOSION;
			else
				enemyBatState = EF_FLY;
		}
	}
}

void EnemyBat::Move(LOOKING_DIRECTION direction, float dt) {
	
	iPoint pospl;
	pospl.x = App->player->playerData.pos.x / App->map->data.tilesets.start->data->tile_width;
	pospl.y = App->player->playerData.pos.y / App->map->data.tilesets.start->data->tile_height;
		
	iPoint posen;
	posen.x = pos.x / App->map->data.tilesets.start->data->tile_width;
	posen.y = pos.y / App->map->data.tilesets.start->data->tile_height;
	
	if (abs(pospl.x - posen.x) < 4)
	{
		App->pathfinding->CreatePath(posen, pospl);

		lastpath = App->pathfinding->GetLastPath();

		lastpath->Flip();

		lastpath->Pop(nextpos);
		lastpath->Pop(nextpos);

		if (nextpos.x > posen.x)
		{
			pos.x += 200.0 * App->dt;
		}

		else if (nextpos.x < posen.x)
		{
			pos.x -= 200.0 * App->dt;
		}

		if (nextpos.y > posen.y)
		{
			pos.y += 200.0 * App->dt;
		}

		else if (nextpos.y < posen.y)
		{
			pos.y -= 200.0 * App->dt;
		}
	}
	
	/*if (enemyBatLookingDirection == L_RIGHT) {
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
	}*/
}

void EnemyBat::ChargeAnimations() {

	Still.PushBack({ 33, 1, 31, 26 });
	Still.PushBack({ 65, 1, 31, 26 });
	Still.PushBack({ 97, 1, 31, 26 });
	Still.speed = 0.5f;

	Fly_R.PushBack({ 41, 33, 17, 26 });
	Fly_R.PushBack({ 73, 33, 17, 26 });
	Fly_R.PushBack({ 106, 33, 17, 26 });
	Fly_R.speed = 0.5f;

	Fly_L.PushBack({ 38, 97, 17, 26 });
	Fly_L.PushBack({ 70, 97, 17, 26 });
	Fly_L.PushBack({ 101, 97, 17, 26 });
	Fly_L.speed = 0.5f;

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
		App->render->Blit(texturaEnemy, pos.x, pos.y, &enemyAnim.GetCurrentFrame());
	}
}