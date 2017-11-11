#include "j1App.h"
#include "ModulePlayer.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"

#include "ModuleEnemies.h"

ModulePlayer::ModulePlayer() {}
ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {
	playerData.x = 0;
	playerData.y = 0;
	playerData.w = 40;
	playerData.h = 70;
	playerData.playerState = PLAYER_STATE::STAND;
	playerData.playerSprites = App->tex->Load("textures/characterSprites.png");
	playerData.tempoJump = 0;
	playerData.timeOnAir = 300;
	playerData.contadorAuxiliarAnimacions = 0;
	playerData.lookingWay = LOOKING_DIRECTION::L_RIGHT;
	playerData.tempoTP = 0;
	playerData.tempoDead = -1;

	ChargeAnimations();

	return true;
}

bool ModulePlayer::Update(float dt) {
	MovimentPlayer();
	DrawPlayer();
	if (App->input->GetKey(SDL_SCANCODE_0) == j1KeyState::KEY_DOWN)
		App->enemies->addEnemy(E_WALKER, playerData.x + 50, playerData.y);
	return true;
}

bool ModulePlayer::CleanUp() {
	return true;
}

void ModulePlayer::DrawPlayer() {
	switch (playerData.playerState) {

	case PLAYER_STATE::STAND:
		if (playerData.lookingWay == L_RIGHT) {
			if (col[0] == true) {
				if (playerData.contadorAuxiliarAnimacions != -1) {
					playerData.playerAnim = playerData.playerAnimation_STAND_R;
					playerData.contadorAuxiliarAnimacions = -1;
				}
			}
			else {
				if (playerData.contadorAuxiliarAnimacions != 9) {
					playerData.playerAnim = playerData.playerAnimation_CAIENT_R;
					playerData.contadorAuxiliarAnimacions = 9;
				}
			}

		}
		else if (playerData.lookingWay == L_LEFT) {
			if (col[0] == true) {
				if (playerData.contadorAuxiliarAnimacions != 0) {
					playerData.playerAnim = playerData.playerAnimation_STAND_L;
					playerData.contadorAuxiliarAnimacions = 0;
				}
			}
			else {
				if (playerData.contadorAuxiliarAnimacions != 10) {
					playerData.playerAnim = playerData.playerAnimation_CAIENT_L;
					playerData.contadorAuxiliarAnimacions = 10;
				}
			}
		}
		break;

	case PLAYER_STATE::RUNING:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contadorAuxiliarAnimacions != 1) {
				playerData.playerAnim = playerData.playerAnimation_RUN_R;
				playerData.contadorAuxiliarAnimacions = 1;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contadorAuxiliarAnimacions != 2) {
				playerData.playerAnim = playerData.playerAnimation_RUN_L;
				playerData.contadorAuxiliarAnimacions = 2;
			}
		}
		break;

	case PLAYER_STATE::JUMPING:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contadorAuxiliarAnimacions != 3) {
				playerData.playerAnim = playerData.playerAnimation_JUMP_R;
				playerData.contadorAuxiliarAnimacions = 3;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contadorAuxiliarAnimacions != 4) {
				playerData.playerAnim = playerData.playerAnimation_JUMP_L;
				playerData.contadorAuxiliarAnimacions = 4;
			}
		}
		break;
		
	case PLAYER_STATE::HABILITY_Q:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contadorAuxiliarAnimacions != 7) {
				playerData.playerAnim = playerData.playerAnimation_GHOST_R;
				playerData.contadorAuxiliarAnimacions = 7;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contadorAuxiliarAnimacions != 8) {
				playerData.playerAnim = playerData.playerAnimation_GHOST_L;
				playerData.contadorAuxiliarAnimacions = 8;
			}
		}
			
		break;
	}

	if (playerData.playerSprites != nullptr)
		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerAnim.GetCurrentFrame());
}

void ModulePlayer::MovimentPlayer() {
	col[0] = App->map->IsCollidingWithTerrain(playerData.x + playerData.w / 2, playerData.y + playerData.h, DOWN);
	col[1] = App->map->IsCollidingWithTerrain(playerData.x + playerData.w / 2, playerData.y, UP);
	col[2] = App->map->IsCollidingWithTerrain(playerData.x, playerData.y + playerData.h / 2, LEFT);
	col[3] = App->map->IsCollidingWithTerrain(playerData.x + playerData.w, playerData.y + playerData.h / 2, RIGHT);
	// 0-Down, 1-Up, 2-Left, 3-Right

	switch (playerData.playerState) {

	case STAND:
		AccioMovLaterals(col);
		AccioMovJump_Gravity(col);
		AccioTp();
		break;

	case RUNING:
		if (!AccioMovLaterals(col))
			playerData.playerState = PLAYER_STATE::STAND;
		AccioMovJump_Gravity(col);
		AccioTp();
		break;

	case JUMPING:
		if (playerData.tempoJump < SDL_GetTicks())
			playerData.playerState = STAND;
		else if ((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir / 3) {
			if (col[1] == false)
				playerData.y -= 8.0f;
		}
		else if (((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir / 2)) {
			if (col[1] == false)
				playerData.y -= 12.0f;
		}
		else if (((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir)) {
			if (col[1] == false)
				playerData.y -= 15.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false)
			playerData.x += 6.0f;
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false)
			playerData.x -= 6.0f;

		AccioTp();
		break;

	case HABILITY_Q:
		App->render->Blit(playerData.playerSprites, playerData.x + (playerData.w / 2) + 100 - 50/*tamany del fum / 2*/, playerData.y - (playerData.h / 2) - 10, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x + (playerData.w / 2) - 100 - 50, playerData.y - (playerData.h / 2) - 10, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x - (playerData.w / 2) - 10, playerData.y - 100 - 50, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x - (playerData.w / 2) - 10, playerData.y + 100 - 50, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
			playerData.x += 100.0f + (playerData.w / 2);
			playerData.tempoTP = SDL_GetTicks() + 1000;
			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
			playerData.x -= 100.0f + (playerData.w / 2);
			playerData.tempoTP = SDL_GetTicks() + 1000;	
			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT) {
			playerData.y -= 100.0f;
			playerData.tempoTP = SDL_GetTicks() + 1000;
			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT) {
			playerData.y += 100.0f;
			playerData.tempoTP = SDL_GetTicks() + 1000;
			playerData.playerState = PLAYER_STATE::STAND;
		}
		break;

	case DEAD:
		if (playerData.tempoDead == -1) {
			playerData.tempoDead = SDL_GetTicks() + 500;
			playerData.playerAnim = playerData.playerAnimation_DEAD;
			playerData.contadorAuxiliarAnimacions = 5;
		}
		else {
			if (playerData.tempoDead < SDL_GetTicks()) {
				playerData.tempoDead = -1;
				playerData.playerState = PLAYER_STATE::STAND;
				playerData.playerAnim = playerData.playerAnimation_DEAD;
				playerData.x = 0;
				playerData.y = 0;
				App->render->camera.x = 0;
				App->render->camera.y = 0;
			}
		}
		break;

	default:
		playerData.playerState = STAND;
		break;
	}

	if (col[0] == true && col[1] == true && col[2] == true && col[3]) {
		playerData.playerState = PLAYER_STATE::DEAD;
	}
}

void ModulePlayer::ChargeAnimations() {
	playerData.playerAnimation_STAND_R.PushBack({ 11,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 57,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 102,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 147,20, 40, 70 });
	playerData.playerAnimation_STAND_R.speed = 0.25f;

	playerData.playerAnimation_STAND_L.PushBack({ 227,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 272,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 317,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 362,20, 40, 70 });
	playerData.playerAnimation_STAND_L.speed = 0.25f;

	playerData.playerAnimation_RUN_R.PushBack({ 9,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 82,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 153,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 220,108, 50, 60 });
	playerData.playerAnimation_RUN_R.speed = 0.4f;

	playerData.playerAnimation_RUN_L.PushBack({ 9,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 70,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 139,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 204,177, 50, 60 });
	playerData.playerAnimation_RUN_L.speed = 0.4f;

	playerData.playerAnimation_JUMP_R.PushBack({ 139,248, 40, 80 });
	playerData.playerAnimation_JUMP_R.PushBack({ 188,248, 40, 80 });
	playerData.playerAnimation_JUMP_R.PushBack({ 233,248, 40, 80 });
	playerData.playerAnimation_JUMP_R.PushBack({ 283,248, 40, 80 });
	playerData.playerAnimation_JUMP_R.speed = 0.10f;
	playerData.playerAnimation_JUMP_R.loop = false;

	playerData.playerAnimation_JUMP_L.PushBack({ 219,346, 40, 80 });
	playerData.playerAnimation_JUMP_L.PushBack({ 171,346, 40, 80 });
	playerData.playerAnimation_JUMP_L.PushBack({ 125,346, 40, 80 });
	playerData.playerAnimation_JUMP_L.PushBack({ 59,346, 40, 80 });
	playerData.playerAnimation_JUMP_L.speed = 0.10f;
	playerData.playerAnimation_JUMP_L.loop = false;

	playerData.playerAnimation_DEAD.PushBack({ 9,495, 100, 120 });
	playerData.playerAnimation_DEAD.PushBack({ 137,495, 100, 120 });
	playerData.playerAnimation_DEAD.PushBack({ 250,495, 100, 120 });
	playerData.playerAnimation_DEAD.speed = 0.05f;

	playerData.playerAnimation_TP_SMOKE.PushBack({ 9,495, 100, 120 });
	playerData.playerAnimation_TP_SMOKE.PushBack({ 137,495, 100, 120 });
	playerData.playerAnimation_TP_SMOKE.PushBack({ 250,495, 100, 120 });
	playerData.playerAnimation_TP_SMOKE.speed = 0.05f;

	playerData.playerAnimation_GHOST_R.PushBack({ 20,442,32,40 });
	playerData.playerAnimation_GHOST_R.PushBack({ 59,442,32,40 });
	playerData.playerAnimation_GHOST_R.PushBack({ 98,442,32,40 });
	playerData.playerAnimation_GHOST_R.PushBack({ 132,442,32,40 });
	playerData.playerAnimation_GHOST_R.speed = 0.25f;

	playerData.playerAnimation_GHOST_L.PushBack({ 211,442,32,40 });
	playerData.playerAnimation_GHOST_L.PushBack({ 245,442,32,40 });
	playerData.playerAnimation_GHOST_L.PushBack({ 284,442,32,40 });
	playerData.playerAnimation_GHOST_L.PushBack({ 323,442,32,40 });
	playerData.playerAnimation_GHOST_L.speed = 0.25f;

	playerData.playerAnimation_CAIENT_R.PushBack({ 283, 247, 50, 80 });
	playerData.playerAnimation_CAIENT_R.loop = false;

	playerData.playerAnimation_CAIENT_L.PushBack({ 59, 345, 50, 80 });
	playerData.playerAnimation_CAIENT_L.loop = false;
}


bool ModulePlayer::AccioMovLaterals(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false) {
		if ((playerData.playerState != JUMPING || playerData.playerState != DEAD) && col[0] == true)
			playerData.playerState = RUNING;
		playerData.x += 6.0f;
		playerData.lookingWay = L_RIGHT;
		ret = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false) {
		if ((playerData.playerState != JUMPING || playerData.playerState != DEAD) && col[0] == true)
			playerData.playerState = RUNING;
		playerData.x -= 6.0f;
		playerData.lookingWay = L_LEFT;
		ret = true;
	}
	return ret;
}

bool ModulePlayer::AccioMovJump_Gravity(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir  && col[0] == true) {
		playerData.playerState = JUMPING;
		playerData.tempoJump = SDL_GetTicks() + playerData.timeOnAir;
		ret = true;
	}
	else if (col[0] == false) {
		playerData.y += 10.0f;
		if (playerData.y > App->map->data.tile_height * App->map->data.height)
			playerData.playerState = PLAYER_STATE::DEAD;
	}

	return ret;
}

void ModulePlayer::AccioTp() {
	if (App->input->GetKey(SDL_SCANCODE_Q) == j1KeyState::KEY_DOWN) {
		if (playerData.tempoTP < SDL_GetTicks()) {
			playerData.playerState = PLAYER_STATE::HABILITY_Q;
		}
	}
}