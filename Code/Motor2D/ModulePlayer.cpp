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
	playerData.playerRect = { 27,16, 22, 50 };
	playerData.playerSprites = App->tex->Load("textures/linkSprites.png");
	playerData.tempoJump = 0;
	playerData.timeOnAir = 200;
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
	if (playerData.playerSprites != nullptr)
		App->render->Blit(playerData.playerSprites, playerData.x, playerData.y, &playerData.playerRect);
}

void ModulePlayer::MovimentPlayer() {
	bool col[4] = { App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w / 2 , playerData.y + playerData.playerRect.h, DOWN),
		App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w / 2 , playerData.y, UP),
		App->map->IsCollidingWithTerrain(playerData.x, playerData.y + playerData.playerRect.h / 2, LEFT),
		App->map->IsCollidingWithTerrain(playerData.x + playerData.playerRect.w , playerData.y + playerData.playerRect.h / 2, RIGHT) };
	// 0-Down, 1-Up, 2-Left, 3-Right

	switch (playerData.playerState) {
	case STAND:
		if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir * 3 && col[0] == true) {
			playerData.playerState = JUMPING;
			playerData.tempoJump = SDL_GetTicks() + playerData.timeOnAir;
		}
		else if (col[0] == false)
			playerData.y += 0.5f;
		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false) {
			if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
				playerData.playerState = RUNING;
			playerData.x += 0.5f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false) {
			if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
				playerData.playerState = RUNING;
			playerData.x -= 0.5f;
		}
		break;

	case RUNING:
		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false) {
			if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
				playerData.playerState = RUNING;
			playerData.x += 0.5f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false) {
			if (playerData.playerState != JUMPING || playerData.playerState != DEAD)
				playerData.playerState = RUNING;
			playerData.x -= 0.5f;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && playerData.tempoJump < SDL_GetTicks() - playerData.timeOnAir * 3 && col[0] == true) {
			playerData.playerState = JUMPING;
			playerData.tempoJump = SDL_GetTicks() + playerData.timeOnAir;
		}
		else if (col[0] == false)
			playerData.y += 0.5f;
		break;

	case JUMPING:
		if (playerData.tempoJump < SDL_GetTicks())
			playerData.playerState = STAND;
		else if ((playerData.tempoJump - SDL_GetTicks()) < playerData.timeOnAir)
			if (col[1] == false)
				playerData.y -= 0.8f;

		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && col[3] == false)
			playerData.x += 0.5f;
		else if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && col[2] == false)
			playerData.x -= 0.5f;

		break;
	case DEAD:

		break;
	default:
		playerData.playerState = STAND;
		break;
	}
}