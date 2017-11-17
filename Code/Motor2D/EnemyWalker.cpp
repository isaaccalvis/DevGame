#include "EnemyWalker.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"

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
	canAtacPlayer = false;
}

EnemyWalker::~EnemyWalker() {}

void EnemyWalker::Update() {
	UpdateInfo();
	Move(enemyWalkerLookingDirection);
	Draw();
}

void EnemyWalker::UpdateInfo() {
	// MIRAR CAP AL PLAYER
	if (App->player->playerData.x > x)
		enemyWalkerLookingDirection = L_RIGHT;
	else
		enemyWalkerLookingDirection = L_LEFT;
	// MIRAR SI TOCA AL PLAYER
	if (enemyWalkerState != E_JUMP && enemyWalkerState != E_DEAD && enemyWalkerState != E_SMOKE && enemyWalkerState != E_ATAC) {
		printf_s("a");
		if (enemyWalkerLookingDirection == L_RIGHT) {
			if (App->player->playerData.x - x < DISTANCIA_MIN_ATAC)
				if (App->player->playerData.y < y) {
					if (App->player->playerData.y - y > -10 && y - App->player->playerData.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
				else {
					if (y - App->player->playerData.y > -10 && App->player->playerData.y - y < 10) {
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
			if (x - App->player->playerData.x < DISTANCIA_MIN_ATAC)
				if (App->player->playerData.y < y) {
					if (App->player->playerData.y - y > -10 && y - App->player->playerData.y < 10) {
						enemyWalkerState = E_ATAC;
						tempoAtac = SDL_GetTicks() + 600;
					}
					else
						enemyWalkerState = E_STAND;
				}
				else {
					if (y - App->player->playerData.y > -10 && App->player->playerData.y - y < 10) {
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
	col[0] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)x, (int)y, (int)w, (int)h }, DOWN);
	col[1] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)x, (int)y, (int)w, (int)h }, UP);
	col[2] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)x, (int)y, (int)w, (int)h }, LEFT);
	col[3] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)x, (int)y, (int)w, (int)h }, RIGHT);

	//col[0] = App->map->IsCollidingWithTerrain(x + w / 2, y + h, DOWN);
	//col[1] = App->map->IsCollidingWithTerrain(x + w / 2, y, UP);
	//col[2] = App->map->IsCollidingWithTerrain(x, y + h / 2, LEFT);
	//col[3] = App->map->IsCollidingWithTerrain(x + w, y + h / 2, RIGHT);

	walkable[0] = App->map->IsCollidingWithWalkableByEnemy(x + w / 2, y + h, DOWN);
	walkable[1] = App->map->IsCollidingWithWalkableByEnemy(x + w / 2, y, UP);
	walkable[2] = App->map->IsCollidingWithWalkableByEnemy(x, y + h / 2, LEFT);
	walkable[3] = App->map->IsCollidingWithWalkableByEnemy(x + w, y + h / 2, RIGHT);
}
// TXELL SEXY
void EnemyWalker::Move(LOOKING_DIRECTION direction) {
	if (enemyWalkerLookingDirection == L_RIGHT) {
		switch (enemyWalkerState) {
		case E_STAND:
			gravityFall();
			break;
		case E_WALK:
			gravityFall();
			if (walkable[3] == true)
				x += 3;
			else
				if (col[0] == true)
					enemyWalkerState = E_JUMP;
			break;
		case E_JUMP:
			Jump(L_RIGHT);
			break;
		case E_ATAC:
			if (tempoAtac < SDL_GetTicks())
				enemyWalkerState = E_STAND;
			else if (tempoAtac - SDL_GetTicks() < 200)
				App->player->receivDamageByPosition(SDL_Rect{ (int)(x + w / 2),(int)y,(int)w,(int)h });
			break;
		case E_DEAD:
			gravityFall();
			break;
		case E_SMOKE:
			if (tempoSmokeJump < SDL_GetTicks())
				enemyWalkerState = E_STAND;
			break;
		}
	}
	else {
		switch (enemyWalkerState) {
		case E_STAND:
			gravityFall();

			break;
		case E_WALK:
			gravityFall();
			if (walkable[2] == true)
				x -= 3;
			else
				if (col[0] == true)
					enemyWalkerState = E_JUMP;
			break;
		case E_JUMP:
			Jump(L_LEFT);
			break;
		case E_ATAC:
			if (tempoAtac < SDL_GetTicks())
				enemyWalkerState = E_STAND;
			else if (tempoAtac - SDL_GetTicks() < 200)
				App->player->receivDamageByPosition(SDL_Rect{ (int)(x - w / 2),(int)y,(int)w,(int)h });
			break;
		case E_DEAD:
			gravityFall();
			break;
		case E_SMOKE:
			if (tempoSmokeJump < SDL_GetTicks())
				enemyWalkerState = E_STAND;
			break;
		}
	}
}

void EnemyWalker::Jump(LOOKING_DIRECTION direction) {
	// SEARCH NEXT BLOC TO JUMP
	float nx = -1;
	float ny = -1;
	if (nx == -1) {
		if (enemyWalkerLookingDirection == L_RIGHT) {
			for (int i = -4; i < 4; i++)
				for (int j = 3; j > 1; j--) {
					if (App->map->IsCollidingWithWalkableByEnemy(x + i, y + j, RIGHT) == true) {
						nx = x + i * App->map->data.tile_width;
						ny = y + j * App->map->data.tile_height;
						i = 4;
						j = 1;
					}
				}
		}
		else {
			for (int i = -4; i < 4; i++)
				for (int j = -3; j < 0; j++) {
					if (App->map->IsCollidingWithWalkableByEnemy(x + i, y + j, LEFT) == true) {
						nx = x + i * App->map->data.tile_width;
						ny = y + j * App->map->data.tile_height;
						j = 0;
						i = 4;
					}
				}
		}
	}

	if (nx != -1) {
		float xRes = nx - this->x;
		float yRes = ny - this->y;
		if (xRes < 0) {
			x -= xRes / 3;
			y -= yRes / 3;
			enemyWalkerState = E_SMOKE;
			tempoSmokeJump = SDL_GetTicks() + 1000;
		}
		else {
			x -= xRes / 3;
			y -= yRes / 3;
			enemyWalkerState = E_SMOKE;
			tempoSmokeJump = SDL_GetTicks() + 1000;
		}
		printf_s("%f %f -- %f %f\n\n ", nx, ny, xRes / 3, yRes / 3);
	}

	//if (walkable[0] == true && walkable[3] == true && walkable[2] == true) {
	//	enemyWalkerState = E_SMOKE;

	//}
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
	Atac_R.speed = 0.15f;
	Atac_R.loop = false;

	Atac_L.PushBack({ 436, 160, 50, 70 });
	Atac_L.PushBack({ 381, 160, 50, 70 });
	Atac_L.PushBack({ 322, 160, 50, 70 });
	Atac_L.PushBack({ 258, 160, 50, 70 });
	Atac_L.speed = 0.15f;
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
		App->render->Blit(texturaEnemy, x, y, &enemyAnim.GetCurrentFrame());
	}
}

void EnemyWalker::gravityFall() {
	if (col[0] == false) {
		y += 5;
	}
}