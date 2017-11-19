#include "EnemyWalker.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"
#include "ModulePlayer.h"

EnemyWalker::EnemyWalker(float x, float y) {
	pos.x = x;
	pos.y = y;
	w = 50;
	h = 65;
	texturaEnemy = App->tex->Load("textures/Enemy_Walker_1.png");
	ChargeAnimations();
	enemyWalkerLookingDirection = L_LEFT;
	enemyWalkerState = E_STAND;
	controladorAnimations = 0;
	canAtacPlayer = false;
}

EnemyWalker::~EnemyWalker() {}

void EnemyWalker::Update(float dt) {
	this->dt = dt;
	UpdateInfo();
	Move(enemyWalkerLookingDirection);
	Draw();
}

void EnemyWalker::UpdateInfo() {
	// MIRAR CAP AL PLAYER
	if (App->player->playerData.x > pos.x)
		enemyWalkerLookingDirection = L_RIGHT;
	else
		enemyWalkerLookingDirection = L_LEFT;
	// MIRAR SI TOCA AL PLAYER
	if (enemyWalkerState != E_JUMP && enemyWalkerState != E_DEAD && enemyWalkerState != E_SMOKE && enemyWalkerState != E_ATAC) {
		if (enemyWalkerLookingDirection == L_RIGHT) {
			if (App->player->playerData.x - pos.x < DISTANCIA_MIN_ATAC)
				if (App->player->playerData.y < pos.y) {
					if (App->player->playerData.y - pos.y > -10 && pos.y - App->player->playerData.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
				else {
					if (pos.y - App->player->playerData.y > -10 && App->player->playerData.y - pos.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
			else
				enemyWalkerState = E_WALK;
		}
		else {
			if (pos.x - App->player->playerData.x < DISTANCIA_MIN_ATAC)
				if (App->player->playerData.y < pos.y) {
					if (App->player->playerData.y - pos.y > -10 && pos.y - App->player->playerData.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
				else {
					if (pos.y - App->player->playerData.y > -10 && App->player->playerData.y - pos.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
			else
				enemyWalkerState = E_WALK;
		}
	}

	col[0] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)pos.x, (int)pos.y, (int)w, (int)h }, DOWN);
	col[1] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)pos.x, (int)pos.y, (int)w, (int)h }, UP);
	col[2] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)pos.x, (int)pos.y, (int)w, (int)h }, LEFT);
	col[3] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)pos.x, (int)pos.y, (int)w, (int)h }, RIGHT);

	//walkable[0] = App->map->IsCollidingWithWalkableByEnemy(pos.x + w , pos.y + h, DOWN);
	//walkable[1] = App->map->IsCollidingWithWalkableByEnemy(pos.x + w , pos.y, UP);
	//walkable[2] = App->map->IsCollidingWithWalkableByEnemy(pos.x, pos.y + h , LEFT);
	//walkable[3] = App->map->IsCollidingWithWalkableByEnemy(pos.x + w, pos.y + h, RIGHT);
}

void EnemyWalker::Move(LOOKING_DIRECTION direction)
{
	iPoint pospl;
	pospl.x = App->player->playerData.pos.x / App->map->data.tilesets.start->data->tile_width;
	pospl.y = App->player->playerData.pos.y / App->map->data.tilesets.start->data->tile_height;

	iPoint posen;
	posen.x = pos.x / App->map->data.tilesets.start->data->tile_width;
	posen.y = pos.y / App->map->data.tilesets.start->data->tile_height;

	if (enemyWalkerState == E_ATAC) {

		if (tempoAtac < SDL_GetTicks())
			enemyWalkerState = E_STAND;
		else if (tempoAtac - SDL_GetTicks() < 200)
			App->player->receiveDamageByPosition(SDL_Rect{ (int)(pos.x - w / 2),(int)pos.y,(int)w,(int)h });
	}
	else if (abs(pospl.x - posen.x) < 3)
	{
		App->pathfinding->CreatePath(posen, pospl, ENEMY_TYPES::E_WALKER);

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
	}

	gravityFall();
}

void EnemyWalker::gravityFall() {
	if (col[0] == false) 
		pos.y += 200.0 * App->dt;
}

void EnemyWalker::ChargeAnimations() {
	Stand_R.PushBack({ 1, 0, 50, 65 });
	Stand_R.PushBack({ 56, 0, 50, 65 });
	Stand_R.speed = 0.08f;

	Stand_L.PushBack({ 436, 0, 50, 65 });
	Stand_L.PushBack({ 387, 0, 50, 65 });
	Stand_L.speed = 0.08f;

	Walk_R.PushBack({ 0, 70, 42, 65 });
	Walk_R.PushBack({ 54, 70, 42, 65 });
	Walk_R.PushBack({ 102, 70, 42, 65 });
	Walk_R.PushBack({ 154, 70, 42, 65 });
	Walk_R.speed = 0.25f;

	Walk_L.PushBack({ 444, 70, 42, 65 });
	Walk_L.PushBack({ 391, 70, 42, 65 });
	Walk_L.PushBack({ 340, 70, 42, 65 });
	Walk_L.PushBack({ 290, 70, 42, 65 });
	Walk_L.speed = 0.25f;

	Jump_R.PushBack({ 1, 269, 50 , 64 });
	Jump_R.PushBack({ 58, 269, 50 , 64 });
	Jump_R.PushBack({ 112, 269, 50 , 64 });
	Jump_R.PushBack({ 164, 269, 50 , 64 });
	Jump_R.loop = false;
	Jump_R.speed = 0.25f;

	Jump_L.PushBack({ 436, 269, 50 , 64 });
	Jump_L.PushBack({ 382, 269, 50 , 64 });
	Jump_L.PushBack({ 329, 269, 50 , 64 });
	Jump_L.PushBack({ 274, 269, 50 , 64 });
	Jump_L.loop = false;
	Jump_L.speed = 0.25f;


	Atac_R.PushBack({ 1, 160, 50, 70 });
	Atac_R.PushBack({ 59, 160, 50, 70 });
	Atac_R.PushBack({ 119, 160, 50, 70 });
	Atac_R.PushBack({ 175, 160, 50, 70 });
	Atac_R.speed = 0.9f;
	Atac_R.loop = false;

	Atac_L.PushBack({ 436, 160, 50, 70 });
	Atac_L.PushBack({ 381, 160, 50, 70 });
	Atac_L.PushBack({ 322, 160, 50, 70 });
	Atac_L.PushBack({ 258, 160, 50, 70 });
	Atac_L.speed = 0.9f;
	Atac_L.loop = false;

	Dead_R.PushBack({ 8, 342, 50, 65 });
	Dead_R.PushBack({ 68, 342, 50, 65 });
	Dead_R.speed = 0.1f;

	Dead_L.PushBack({ 429, 342, 50, 65 });
	Dead_L.PushBack({ 369, 342, 50, 65 });
	Dead_L.speed = 0.1f;

	Smoke.PushBack({201, 0, 73, 85});
	Smoke.PushBack({ 201, 331, 73, 85 });
	Smoke.speed = 0.025f;
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
			case E_SMOKE:
				if (controladorAnimations != 10) {
					enemyAnim = Smoke;
					controladorAnimations = 10;
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
			case E_SMOKE:
				if (controladorAnimations != 10) {
					enemyAnim = Smoke;
					controladorAnimations = 10;
				}
				break;
			}
		App->render->Blit(texturaEnemy, pos.x, pos.y, &enemyAnim.GetCurrentFrame());
	}
}