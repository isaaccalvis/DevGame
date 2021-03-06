#include "j1App.h"
#include "ModulePlayer.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"

#include "ModuleGui.h"

#include "ModuleEnemies.h"

ModulePlayer::ModulePlayer() 
{ 
	name.create("player");
}
ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {

	LoadPlayerProperties();
	ChargeAnimations();

	return true;
}

bool ModulePlayer::Update(float dt) {
	this->dt = dt;
	MovementPlayer();
	DrawPlayer();

	App->render->camera.x = App->render->cam.x;
	App->render->camera.y = App->render->cam.y;
	playerData.pos.x = playerData.x;
	playerData.pos.y = playerData.y;

	return true;
}

bool ModulePlayer::CleanUp() {
	App->tex->UnLoad(playerData.playerSprites);
	return true;
}

void ModulePlayer::LoadPLayerTexture() {
	playerData.playerSprites = App->tex->Load("textures/characterSprites.png");
}

bool ModulePlayer::Load(pugi::xml_node& data)
{
	playerData.x = data.child("pos").attribute("x").as_int();
	playerData.y = data.child("pos").attribute("y").as_int();

	return true;
}

bool ModulePlayer::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("pos");

	position.append_attribute("x") = (int)playerData.x;
	position.append_attribute("y") = (int)playerData.y;

	return true;
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
	colAuxiliar = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)(playerData.x + playerData.w / 4), (int)playerData.y + (int)playerData.h - 1, (int)(playerData.w / 2), 1}, UP);

	while (colAuxiliar == true && App->maxfps != 30) {
		playerData.y--;
		colAuxiliar = App->map->IsCollidingWithTerraint(SDL_Rect{ (int)(playerData.x + playerData.w / 4), (int)playerData.y + (int)playerData.h - 1, (int)(playerData.w / 2), 1 }, UP);
	}

	switch (playerData.playerState) {
	case STAND:
		AccioMovLaterals(col);
		AccioMovJump_Gravity(col);
		ActionTp();
		Attack();
		break;

	case RUNING:
		if (!AccioMovLaterals(col))
			playerData.playerState = PLAYER_STATE::STAND;
		AccioMovJump_Gravity(col);
		ActionTp();
		Attack();
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

		ActionTp();
		Attack();
		break;

	case HABILITY_Q:
		App->render->Blit(playerData.playerSprites, playerData.x + (playerData.w / 2) + 100 - 50, playerData.y - (playerData.h / 2) - 10, &playerData.playerAnimation_TP_SMOKE.GetCurrentFrame());
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
			playerData.contAuxAnim = 5;
		}
		else {
			if (playerData.tempoDead < SDL_GetTicks()) {
				playerData.tempoDead = -1;
				playerData.playerState = PLAYER_STATE::STAND;
				playerData.playerAnim = playerData.playerAnimation_DEAD;
				App->map->CleanUp();
				//App->tex->FreeTextures();
				App->player->LoadPLayerTexture();

				App->map->Load(App->scene->current_map->data.GetString());

				App->enemies->FindEnemies();

				App->render->camera.x = App->render->cam.x = 0;
				App->render->camera.y = App->render->cam.y = 0;

				App->player->SpawnPLayer();

			}
		}
		break;
	case PLAYER_STATE::ATTACK:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.tempoAtac - SDL_GetTicks() < 350 && playerData.tempoAtac - SDL_GetTicks() > 200)
				App->enemies->receiveDamageEnemyAtThisPosition(SDL_Rect{ (int)(playerData.x + playerData.w), (int)playerData.y, (int)(playerData.w / 2), (int)playerData.h });
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.tempoAtac - SDL_GetTicks() < 350 && playerData.tempoAtac - SDL_GetTicks() > 200)
				App->enemies->receiveDamageEnemyAtThisPosition(SDL_Rect{ (int)(playerData.x - (playerData.w / 2)), (int)playerData.y, (int)playerData.w, (int)playerData.h });
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

void ModulePlayer::receiveDamageByPosition(SDL_Rect rect) {
	if (App->scene->godmode == false)
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
	if (col[0] == true && col[2] == true && col[3] == true)
		playerData.y -= 1;
	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir  && col[0] == true) {
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

void ModulePlayer::ActionTp() {
	if (App->input->GetKey(SDL_SCANCODE_Q) == j1KeyState::KEY_DOWN) {
		if (playerData.tempoTP < SDL_GetTicks()) {
			playerData.playerState = PLAYER_STATE::HABILITY_Q;
		}
	}
}

void ModulePlayer::Attack() {
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN) {
		playerData.tempoAtac = SDL_GetTicks() + 400;
		playerData.playerState = PLAYER_STATE::ATTACK;
	}
}

void ModulePlayer::DrawPlayer() 
{
	switch (playerData.playerState) {

	case PLAYER_STATE::STAND:
		if (playerData.lookingWay == L_RIGHT) {
			if (col[0] == true) {
				if (playerData.contAuxAnim != -1) {
					playerData.playerAnim = playerData.playerAnimation_STAND_R;
					playerData.contAuxAnim = -1;
				}
			}
			else {
				if (playerData.contAuxAnim != 9) {
					playerData.playerAnim = playerData.playerAnimation_FALLING_R;
					playerData.contAuxAnim = 9;
				}
			}

		}
		else if (playerData.lookingWay == L_LEFT) {
			if (col[0] == true) {
				if (playerData.contAuxAnim != 0) {
					playerData.playerAnim = playerData.playerAnimation_STAND_L;
					playerData.contAuxAnim = 0;
				}
			}
			else {
				if (playerData.contAuxAnim != 10) {
					playerData.playerAnim = playerData.playerAnimation_FALLING_L;
					playerData.contAuxAnim = 10;
				}
			}
		}
		break;

	case PLAYER_STATE::RUNING:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contAuxAnim != 1) {
				playerData.playerAnim = playerData.playerAnimation_RUN_R;
				playerData.contAuxAnim = 1;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contAuxAnim != 2) {
				playerData.playerAnim = playerData.playerAnimation_RUN_L;
				playerData.contAuxAnim = 2;
			}
		}
		break;

	case PLAYER_STATE::JUMPING:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contAuxAnim != 3) {
				playerData.playerAnim = playerData.playerAnimation_JUMP_R;
				playerData.contAuxAnim = 3;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contAuxAnim != 4) {
				playerData.playerAnim = playerData.playerAnimation_JUMP_L;
				playerData.contAuxAnim = 4;
			}
		}
		break;

	case PLAYER_STATE::HABILITY_Q:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contAuxAnim != 7) {
				playerData.playerAnim = playerData.playerAnimation_GHOST_R;
				playerData.contAuxAnim = 7;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contAuxAnim != 8) {
				playerData.playerAnim = playerData.playerAnimation_GHOST_L;
				playerData.contAuxAnim = 8;
			}
		}
		break;
	case PLAYER_STATE::ATTACK:
		if (playerData.lookingWay == L_RIGHT) {
			if (playerData.contAuxAnim != 11) {
				playerData.playerAnim = playerData.playerAnimation_ATTACK_R;
				playerData.contAuxAnim = 11;
			}
		}
		else if (playerData.lookingWay == L_LEFT) {
			if (playerData.contAuxAnim != 12) {
				playerData.playerAnim = playerData.playerAnimation_ATTACK_L;
				playerData.contAuxAnim = 12;
			}
		}
		break;
	}

	if (playerData.playerSprites != nullptr)
		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerAnim.GetCurrentFrame());
}

void ModulePlayer::LoadPlayerProperties() {
	playerData.x = 0;
	playerData.y = 0;
	playerData.w = 40;
	playerData.h = 70;
	playerData.playerState = PLAYER_STATE::STAND;
	LoadPLayerTexture();
	playerData.tempoJump = 0;
	playerData.timeOnAir = 300;
	playerData.contAuxAnim = 0;
	playerData.lookingWay = LOOKING_DIRECTION::L_RIGHT;
	playerData.tempoTP = 0;
	playerData.tempoDead = -1;
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
	playerData.playerAnimation_RUN_R.speed = 1.0f;

	playerData.playerAnimation_RUN_L.PushBack({ 9,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 70,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 139,177, 50, 60 });
	playerData.playerAnimation_RUN_L.PushBack({ 204,177, 50, 60 });
	playerData.playerAnimation_RUN_L.speed = 1.0f;

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
	playerData.playerAnimation_TP_SMOKE.PushBack({ 275,496, 100, 120 });
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

	playerData.playerAnimation_FALLING_R.PushBack({ 283, 247, 50, 80 });
	playerData.playerAnimation_FALLING_R.loop = false;

	playerData.playerAnimation_FALLING_L.PushBack({ 59, 345, 50, 80 });
	playerData.playerAnimation_FALLING_L.loop = false;

	playerData.playerAnimation_ATTACK_R.PushBack({ 47,643,35 ,78 });
	playerData.playerAnimation_ATTACK_R.PushBack({ 89,643,69 ,78 });
	playerData.playerAnimation_ATTACK_R.speed = 0.9f;
	playerData.playerAnimation_ATTACK_R.loop = false;

	playerData.playerAnimation_ATTACK_L.PushBack({ 252 ,643,35 ,78 });
	playerData.playerAnimation_ATTACK_L.PushBack({ 179 ,643,69 ,78 });
	playerData.playerAnimation_ATTACK_L.speed = 0.9f;
	playerData.playerAnimation_ATTACK_L.loop = false;
}