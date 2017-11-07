#include "j1App.h"
#include "ModulePlayer.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"


ModulePlayer::ModulePlayer() {}
ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {
	playerData.x = 0;
	playerData.y = 0;
	playerData.playerState = PLAYER_STATE::STAND;
	playerData.playerRect = { 0,0, 40, 70 };
	playerData.playerSprites = App->tex->Load("textures/characterSprites.png");
	playerData.tempoJump = 0;
	playerData.timeOnAir = 200;
	playerData.contadorAuxiliarAnimacions = 0;
	playerData.lookingWay = LOOKING_DIRECTION::L_RIGHT;
	playerData.tempoPerTP = 0;

	playerData.playerAnimation_STAND_R.PushBack({ 11,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 57,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 102,20, 40, 70 });
	playerData.playerAnimation_STAND_R.PushBack({ 147,20, 40, 70 });
	playerData.playerAnimation_STAND_R.speed = 0.025f;

	playerData.playerAnimation_STAND_L.PushBack({ 227,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 272,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 317,20, 40, 70 });
	playerData.playerAnimation_STAND_L.PushBack({ 362,20, 40, 70 });
	playerData.playerAnimation_STAND_L.speed = 0.025f;

	playerData.playerAnimation_RUN_R.PushBack({ 9,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 82,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 153,108, 50, 60 });
	playerData.playerAnimation_RUN_R.PushBack({ 220,108, 50, 60 });
	playerData.playerAnimation_RUN_R.speed = 0.025f;

	playerData.playerAnimation_RUN_L.PushBack({ 9,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 70,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 139,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 204,177, 50, 60 });
	playerData.playerAnimation_RUN_L.speed = 0.025f;

	playerData.playerAnimation_JUMP_R.PushBack({ 139,248, 40, 60 });
	playerData.playerAnimation_JUMP_R.PushBack({ 188,248, 40, 60 });
	playerData.playerAnimation_JUMP_R.PushBack({ 233,248, 40, 60 });
	playerData.playerAnimation_JUMP_R.PushBack({ 283,248, 40, 60 });
	playerData.playerAnimation_JUMP_R.speed = 0.025f;

	playerData.playerAnimation_JUMP_L.PushBack({ 219,346, 40, 60 });
	playerData.playerAnimation_JUMP_L.PushBack({ 171,346, 40, 60 });
	playerData.playerAnimation_JUMP_L.PushBack({ 125,346, 40, 60 });
	playerData.playerAnimation_JUMP_L.PushBack({ 59,346, 40, 60 });
	playerData.playerAnimation_JUMP_L.speed = 0.025f;

	playerData.playerAnimation_TP_SMOKE.PushBack({ 9,495, 100, 120 });

	return true;
}

bool ModulePlayer::Update(float dt) {
	MovimentPlayer();
	DrawPlayer();
	return true;
}

bool ModulePlayer::CleanUp() {
	return true;
}

void ModulePlayer::DrawPlayer() {
	switch (playerData.playerState) {

	case PLAYER_STATE::STAND:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contadorAuxiliarAnimacions != -1) {
				playerData.playerAnim = playerData.playerAnimation_STAND_R;
				playerData.contadorAuxiliarAnimacions = -1;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contadorAuxiliarAnimacions != 0) {
				playerData.playerAnim = playerData.playerAnimation_STAND_L;
				playerData.contadorAuxiliarAnimacions = 0;
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
	}

	if (playerData.playerSprites != nullptr)
		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerAnim.GetCurrentFrame());
}

void ModulePlayer::MovimentPlayer() {
	bool col[4] = { App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w / 2 , playerData.y + playerData.playerRect.h, DOWN),
		App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w / 2 , playerData.y, UP),
		App->map->IsCollidingWithTerrain(playerData.x, playerData.y + playerData.playerRect.h / 2, LEFT),
		App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w , playerData.y + playerData.playerRect.h / 2, RIGHT) };
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
		else if ((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir)
			if (col[1] == false)
				playerData.y -= 1.0f;

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false)
			playerData.x += 0.5f;
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false)
			playerData.x -= 0.5f;

		AccioTp();
		break;

	case DEAD:

		break;

	default:
		playerData.playerState = STAND;
		break;
	}
}

bool ModulePlayer::AccioMovLaterals(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false) {
		if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
			playerData.playerState = RUNING;
		playerData.x += 0.5f;
		playerData.lookingWay = L_RIGHT;
		ret = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false) {
		if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
			playerData.playerState = RUNING;
		playerData.x -= 0.5f;
		playerData.lookingWay = L_LEFT;
		ret = true;
	}
	return ret;
}

bool ModulePlayer::AccioMovJump_Gravity(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir * 3 && col[0] == true) {
		playerData.playerState = JUMPING;
		playerData.tempoJump = SDL_GetTicks() + playerData.timeOnAir;
		ret = true;
	}
	else if (col[0] == false)
		playerData.y += 0.5f;

	return ret;
}

bool ModulePlayer::AccioTp() {
	bool ret = false;

	if (App->input->GetKey(SDL_SCANCODE_Q) == j1KeyState::KEY_DOWN) {
		if (playerData.tempoPerTP < SDL_GetTicks()) {
			playerData.tempoPerTP = SDL_GetTicks() + 1000; // temps de cooldown

			int dir = 0;
			while (dir == 0) {
				if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN)
					dir = 1;
				else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_DOWN)
					dir = 2;
				else if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN)
					dir = 3;

				App->render->Blit(playerData.playerSprites, playerData.x + 100, playerData.y, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
				App->render->Blit(playerData.playerSprites, playerData.x - 100, playerData.y, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
				App->render->Blit(playerData.playerSprites, playerData.x, playerData.y + 100, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
			}

			switch (dir) {
			case 0:
				playerData.x += 100.0f;
				break;
			case 1:
				playerData.x -= 100.0f;
				break;
			case 2:
				playerData.y -= 100.0f;
				break;
			}

		}
	}
	return ret;
}

//bool ModulePlayer::AccioTp() {
//	bool ret = false;
//	if (playerData.tempoPerTP > SDL_GetTicks() + 300) {
//		App->render->Blit(playerData.playerSprites, playerData.xFum, playerData.yFum, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
//		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
//	}
//	if (App->input->GetKey(SDL_SCANCODE_Q) == j1KeyState::KEY_DOWN) {
//		playerData.xFum = playerData.x;
//		playerData.yFum = playerData.y;
//		int dir = 0;
//		while (dir == 0) {
//			if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN)
//				dir = 1;
//		}
//
//		if (playerData.tempoPerTP < SDL_GetTicks()) {
//			playerData.tempoPerTP = SDL_GetTicks() + 1000; // temps de cooldown
//			if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT) {
//				playerData.y -= 100.0f;
//				ret = true;
//			}
//			else if (playerData.lookingWay == L_RIGHT) {
//				playerData.x += 100.0f;
//				ret = true;
//			}
//			else {
//				playerData.x -= 100.0f;
//				ret = true;
//			}
//		}
//	}
//	return ret;
//}