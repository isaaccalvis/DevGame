#include "EnemyBat.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "ModulePlayer.h"

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
	if (canGetCloser)
		getCloser();
	canGetCloser = false;
	Draw();
}

void EnemyBat::UpdateInfo() {
	if (enemyBatState != EF_ATAC)
		if (App->player->playerData.x < pos.x + w &&
			App->player->playerData.x + App->player->playerData.w > pos.x &&
			App->player->playerData.y < pos.y + h &&
			App->player->playerData.h + App->player->playerData.y > pos.y) {
			enemyBatState = EF_ATAC;
			tempoExplosio = SDL_GetTicks() + 1000;
		}
	else
		canAtacPlayer = false;

	if (!canAtacPlayer)
		enemyBatLookingDirection = L_NEUTRAL;
	else {
		if (App->player->playerData.x > pos.x)
			enemyBatLookingDirection = L_RIGHT;
		else
			enemyBatLookingDirection = L_LEFT;
	}
}

void EnemyBat::Move(LOOKING_DIRECTION direction, float dt) {
	iPoint pospl;
	pospl.x = App->player->playerData.pos.x / App->map->data.tilesets.start->data->tile_width;
	pospl.y = App->player->playerData.pos.y / App->map->data.tilesets.start->data->tile_height;

	iPoint posen;
	posen.x = pos.x / App->map->data.tilesets.start->data->tile_width;
	posen.y = pos.y / App->map->data.tilesets.start->data->tile_height;

	if (enemyBatState == EF_ATAC) {
		if (tempoExplosio < SDL_GetTicks())
			isAlive = false;
		else if (tempoExplosio - SDL_GetTicks() < 500) {
			App->player->receiveDamageByPosition(SDL_Rect{ (int)pos.x, (int)pos.y, 32, 63 });
		}
	}

	else if (abs(pospl.x - posen.x) < 4)
	{

		App->pathfinding->CreatePath(posen, pospl, ENEMY_TYPES::E_BAT);

		lastpath = App->pathfinding->GetLastPath();

		lastpath->Flip();

		lastpath->Pop(nextpos);
		lastpath->Pop(nextpos);

		if (nextpos.x > posen.x)
			pos.x += 200.0 * App->dt;
		else if (nextpos.x < posen.x)
			pos.x -= 200.0 * App->dt;

		if (nextpos.y > posen.y)
			pos.y += 200.0 * App->dt;
		else if (nextpos.y < posen.y)
			pos.y -= 200.0 * App->dt;

		canGetCloser = true;
	}
}

void EnemyBat::Draw() {
	if (texturaEnemy != nullptr) {
		if (enemyBatState == EF_ATAC) {
			if (tempoExplosio - SDL_GetTicks() < 500) {
				if (controladorAnimations != 5) {
					enemyAnim = Explosion;
					controladorAnimations = 5;
				}
			}
			else {
				enemyAnim = Still;
			}
		}
		else if (enemyBatLookingDirection == L_NEUTRAL) {
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
			}
		else
			switch (enemyBatState) {
			case EF_FLY:
				if (controladorAnimations != 1) {
					enemyAnim = Fly_L;
					controladorAnimations = 1;
				}
				break;
			}
		App->render->Blit(texturaEnemy, pos.x, pos.y, &enemyAnim.GetCurrentFrame());
	}
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

	Explosion.PushBack({ 0,136,32,32 });
	Explosion.PushBack({ 42,136,32,32 });
	Explosion.speed = 0.25f;
}

void EnemyBat::getCloser()
{
	if (pos.x < App->player->playerData.pos.x)
		pos.x += 200.0 * App->dt;
	else if (pos.x > App->player->playerData.pos.x + App->player->playerData.w - 5)
		pos.x -= 200.0 * App->dt;
	if (pos.y < App->player->playerData.pos.y)
		pos.y += 200.0 * App->dt;
	else if (pos.y > App->player->playerData.pos.y + App->player->playerData.h)
		pos.y -= 200.0 * App->dt;
}