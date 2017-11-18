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
	this->dt = dt;
	MovementPlayer();
	DrawPlayer();
	if (App->input->GetKey(SDL_SCANCODE_1) == j1KeyState::KEY_DOWN)
		App->enemies->addEnemy(E_WALKER, playerData.x + 50, playerData.y);

	if (App->input->GetKey(SDL_SCANCODE_2) == j1KeyState::KEY_DOWN)
		App->enemies->addEnemy(E_BAT, playerData.x + 50, playerData.y);

	App->render->camera.x = App->render->cam.x;
	App->render->camera.y = App->render->cam.y;
	playerData.pos.x = playerData.x;
	playerData.pos.y = playerData.y;

	return true;
}

bool ModulePlayer::CleanUp() {
	return true;
}

void ModulePlayer::LoadPLayerTexture() {
	playerData.playerSprites = App->tex->Load("textures/characterSprites.png");
}

void ModulePlayer::SpawnPLayer() {
	playerData.x = App->map->data.spawnOnMap.x;
	playerData.y = App->map->data.spawnOnMap.y;
}

void ModulePlayer::MovementPlayer() {
	col[0] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)(playerData.x + playerData.w / 4), (int)playerData.y + (int)playerData.h, (int)(playerData.w / 2), 1 }, DOWN);
	col[1] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)(playerData.x + playerData.w / 4), (int)playerData.y, (int)(playerData.w / 2), 1 }, UP);
	col[2] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)playerData.x, (int)(playerData.y + playerData.h / 4), 1, (int)(playerData.h / 2) }, LEFT);
	col[3] = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)playerData.x + (int)playerData.w, (int)(playerData.y + playerData.h / 4), 1, (int)(playerData.h / 2) }, RIGHT);

	switch (playerData.playerState) {
	case STAND:
		AccioMovLaterals(col);
		AccioMovJump_Gravity(col);
		AccioTp();
		Atac();
		//RecolocarPeusPlayer();
		break;

	case RUNING:
		if (!AccioMovLaterals(col))
			playerData.playerState = PLAYER_STATE::STAND;
		AccioMovJump_Gravity(col);
		AccioTp();
		Atac();
		//RecolocarPeusPlayer();
		break;

	case JUMPING:
		if (playerData.tempoJump < SDL_GetTicks())
			playerData.playerState = STAND;
		else if ((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir / 3) {
			if (col[1] == false)
				playerData.y -= 800.0f * dt;
		}
		else if (((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir / 2)) {
			if (col[1] == false)
				playerData.y -= 800.0f * dt;
		}
		else if (((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir)) {
			if (col[1] == false)
				playerData.y -= 800.0f * dt;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false)
			playerData.x += playerData.speed * dt;
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false)
			playerData.x -= playerData.speed * dt;

		AccioTp();
		Atac();
		break;

	case HABILITY_Q:
		App->render->Blit(playerData.playerSprites, playerData.x + (playerData.w / 2) + 100 - 50/*tamany del fum / 2*/, playerData.y - (playerData.h / 2) - 10, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x + (playerData.w / 2) - 100 - 50, playerData.y - (playerData.h / 2) - 10, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x - (playerData.w / 2) - 10, playerData.y - 100 - 50, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
		App->render->Blit(playerData.playerSprites, playerData.x - (playerData.w / 2) - 10, playerData.y + 100 - 50, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_DOWN) {
			playerData.x += 100.0f + (playerData.w / 2);
			if (App->render->cam.x - App->render->camera.w - 100.0f + (playerData.w / 2) > -(App->map->data.width*App->map->data.tile_width))
				App->render->cam.x -= 100.0f + (playerData.w / 2);
			playerData.tempoTP = SDL_GetTicks() + 1000;
			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_DOWN) {
			playerData.x -= 100.0f + (playerData.w / 2);
			if (App->render->cam.x + 100.0f + (playerData.w / 2) < 0)
				App->render->cam.x += 100.0f + (playerData.w / 2);
			playerData.tempoTP = SDL_GetTicks() + 1000;	

			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN) {
			playerData.y -= 100.0f;
			playerData.tempoTP = SDL_GetTicks() + 1000;
			playerData.playerState = PLAYER_STATE::STAND;
		}
		else if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_DOWN) {
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
				App->render->camera.x = App->render->cam.x = 0;
				App->render->camera.y = App->render->cam.y = 0;
			}
		}
		break;
	case PLAYER_STATE::ATAC:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.tempoAtac - SDL_GetTicks() < 350 && playerData.tempoAtac - SDL_GetTicks() > 200)
				App->enemies->receivDamageEnemyAtThisPosition(SDL_Rect{ (int)(playerData.x + playerData.w), (int)playerData.y, (int)(playerData.w / 2), (int)playerData.h });
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.tempoAtac - SDL_GetTicks() < 350 && playerData.tempoAtac - SDL_GetTicks() > 200)
				App->enemies->receivDamageEnemyAtThisPosition(SDL_Rect{ (int)(playerData.x - (playerData.w / 2)), (int)playerData.y, (int)playerData.w, (int)playerData.h });
		}
		if (playerData.tempoAtac < SDL_GetTicks())
			playerData.playerState = STAND;
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

	playerData.playerAnimation_ATAC_R.PushBack({ 47,643,35 ,78 });
	playerData.playerAnimation_ATAC_R.PushBack({ 89,643,69 ,78 });
	playerData.playerAnimation_ATAC_R.speed = 0.2f;
	playerData.playerAnimation_ATAC_R.loop = false;

	playerData.playerAnimation_ATAC_L.PushBack({ 252 ,643,35 ,78 });
	playerData.playerAnimation_ATAC_L.PushBack({ 179 ,643,69 ,78 });
	playerData.playerAnimation_ATAC_L.speed = 0.2f;
	playerData.playerAnimation_ATAC_L.loop = false;
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
	case PLAYER_STATE::ATAC:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contadorAuxiliarAnimacions != 11) {
				playerData.playerAnim = playerData.playerAnimation_ATAC_R;
				playerData.contadorAuxiliarAnimacions = 11;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contadorAuxiliarAnimacions != 12) {
				playerData.playerAnim = playerData.playerAnimation_ATAC_L;
				playerData.contadorAuxiliarAnimacions = 12;
			}
		}
		break;
	}

	if (playerData.playerSprites != nullptr)
		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerAnim.GetCurrentFrame());
}

void ModulePlayer::receivDamageByPosition(SDL_Rect rect) {
	if (rect.x < playerData.x + playerData.w &&
		rect.x + rect.w > playerData.x &&
		rect.y < playerData.y + playerData.h &&
		rect.h + rect.y > playerData.y)
		playerData.playerState = PLAYER_STATE::DEAD;
}

bool ModulePlayer::AccioMovLaterals(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false) {
		if ((playerData.playerState != JUMPING || playerData.playerState != DEAD) && col[0] == true)
			playerData.playerState = RUNING;
		playerData.x += playerData.speed * dt;
		playerData.lookingWay = L_RIGHT;
		ret = true;
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false) {
		if ((playerData.playerState != JUMPING || playerData.playerState != DEAD) && col[0] == true)
			playerData.playerState = RUNING;
		playerData.x -= playerData.speed * dt;
		playerData.lookingWay = L_LEFT;
		ret = true;
	}
	return ret;
}

bool ModulePlayer::AccioMovJump_Gravity(bool col[4]) {
	bool ret = false;
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_DOWN && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir  && col[0] == true) {
		playerData.playerState = JUMPING;
		playerData.tempoJump = SDL_GetTicks() + playerData.timeOnAir;
		ret = true;
	}
	else if (col[0] == false) {
		playerData.y += playerData.jumpSpeed * dt;
		if (playerData.y > App->map->data.tile_height * (App->map->data.height-1))
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

//void ModulePlayer::RecolocarPeusPlayer() {
//	int comprobacio = ((int)playerData.y + (int)playerData.h) % App->map->data.tile_height;
//	if (col[0] == true && comprobacio < 5) {
//		float aux = (playerData.y + playerData.h) * -1;
//		while (aux < 0) {
//			aux += App->map->data.tile_height;
//		}
//		aux -= App->map->data.tile_height;
//		playerData.y -= aux;
//	}
//}

void ModulePlayer::Atac() {

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN) {
		playerData.tempoAtac = SDL_GetTicks() + 400;
		playerData.playerState = PLAYER_STATE::ATAC;
	}
}