#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "ModulePlayer.h"
#include "ModuleGui.h"
#include "j1FadeToBlack.h"
#include "Menu.h"
#include "gui_image.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	for (pugi::xml_node iterator = config.child("maps"); iterator != nullptr; iterator = iterator.next_sibling("maps"))
	{
		maps.add(iterator.attribute("name").as_string());
	}
	current_map = maps.start;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load(current_map->data.GetString()) == true)
	{
		App->enemies->FindEnemies();
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		App->gui->atlas = App->tex->Load("textures/Game_Gui_Buttons.png");

		if (current_map->next == nullptr)
		{
			moneda_1 = App->gui->addImageOnMap( 933, 120, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_2 = App->gui->addImageOnMap( 1600, 600, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_3 = App->gui->addImageOnMap( 2700, 333, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_4 = App->gui->addImageOnMap( 3400, 472, { 1060, 72, 33, 33 }, App->gui->atlas, this);
		}

		else
		{
			moneda_1 = App->gui->addImageOnMap( 300, 530, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_2 = App->gui->addImageOnMap( 1500, 156, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_3 = App->gui->addImageOnMap( 3000, 382, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_4 = App->gui->addImageOnMap( 3700, 450, { 1060, 72, 33, 33 }, App->gui->atlas, this);
		}

		gear = App->gui->addButton(955, 15, { 1077, 272, 34, 34 }, App->gui->atlas, nullptr, nullptr, { 1077, 272, 34, 34 }, { 1077, 272, 34, 34 }, this);

		save_button = App->gui->addButton(900, 15, { 1084, 374, 27, 27 }, App->gui->atlas, nullptr, nullptr, { 1084, 374, 27, 27 }, { 1084, 374, 27, 27 }, this);

		RELEASE_ARRAY(data);
	}

//	debug_tex = App->tex->Load("maps/graveyard_tile.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	if (App->menu->need_load)
	{
		App->LoadGame();
		App->menu->need_load = false;

		App->gui->atlas = App->tex->Load("textures/Game_Gui_Buttons.png");

		gear = App->gui->addButton(955, 15, { 1077, 272, 34, 34 }, App->gui->atlas, nullptr, nullptr, { 1077, 272, 34, 34 }, { 1077, 272, 34, 34 }, this);

		save_button = App->gui->addButton(900, 15, { 1084, 374, 27, 27 }, App->gui->atlas, nullptr, nullptr, { 1084, 374, 27, 27 }, { 1084, 374, 27, 27 }, this);
	}

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	ChangeScene();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->map->CleanUp();
		App->tex->FreeTextures();
		App->player->LoadPLayerTexture();

		current_map = App->scene->maps.start;

		App->map->Load(current_map->data.GetString());

		App->enemies->FindEnemies();

		App->render->camera.x = App->render->cam.x = 0;
		App->render->camera.y = App->render->cam.y = 0;

		App->player->SpawnPLayer();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->map->CleanUp();
		App->tex->FreeTextures();
		App->player->LoadPLayerTexture();

		App->map->Load(current_map->data.GetString());

		App->enemies->FindEnemies();

		App->render->camera.x = App->render->cam.x = 0;
		App->render->camera.y = App->render->cam.y = 0;

		App->player->SpawnPLayer();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //Colliders/logic
	{
		App->map->logic_draw = !App->map->logic_draw;
	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godmode == false)
			godmode = true;
		else
			godmode = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) //Framerate Cap 30/infinite
	{
		if (App->maxfps == 0)
			App->maxfps = 30;

		else if (App->maxfps == 30)
			App->maxfps = 0;
	}


	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;*/

	if (App->player->playerData.x - (-App->render->cam.x + (App->render->camera.w / 2)) >= 0)
	{
		if (App->render->cam.x - App->render->camera.w > -(App->map->data.width*App->map->data.tile_width) && App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && App->player->col[3] == false)
			App->render->cam.x -= App->player->playerData.speed*dt;
	}
	if (App->player->playerData.x - (-App->render->cam.x + (App->render->camera.w / 2)) <= 0)
	{
		if (App->render->cam.x < 0 && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && App->player->col[2] == false)
			App->render->cam.x += App->player->playerData.speed*dt;
	}

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());*/

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);



	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	}

	App->render->camera.x = App->render->cam.x;
	App->render->camera.y = App->render->cam.y;

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (open_menu)
	{
		App->scene->active = false;
		App->map->active = false;
		App->enemies->active = false;
		App->player->active = false;

		App->fade->FadeToBlack(this, (j1Module*)App->menu, 1.0f);

		App->menu->active = true;

		open_menu = false;
	}

	else if (saveGame) {
		App->SaveGame();
		saveGame = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


bool j1Scene::ChangeScene() {
	if (App->map->IsCollidingWithGoal(App->player->playerData.x + App->player->playerData.w / 2, App->player->playerData.y + App->player->playerData.h, DOWN)
		|| App->map->IsCollidingWithGoal(App->player->playerData.x + App->player->playerData.w / 2, App->player->playerData.y, UP)
		|| App->map->IsCollidingWithGoal(App->player->playerData.x, App->player->playerData.y + App->player->playerData.h / 2, LEFT)
		|| App->map->IsCollidingWithGoal(App->player->playerData.x + App->player->playerData.w, App->player->playerData.y + App->player->playerData.h / 2, RIGHT))
	{

		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data->CleanUp();
		}
		App->gui->gui_objects.clear();

		App->map->CleanUp();
		App->tex->FreeTextures();
		App->player->LoadPLayerTexture();

		if (current_map->next == nullptr)
			current_map = App->scene->maps.start;
		else
			current_map = App->scene->current_map->next;

		App->map->Load(current_map->data.GetString());

		App->enemies->FindEnemies();

		App->gui->atlas = App->tex->Load("textures/Game_Gui_Buttons.png");

		gear = App->gui->addButton(955, 15, { 1077, 272, 34, 34 }, App->gui->atlas, nullptr, nullptr, { 1077, 272, 34, 34 }, { 1077, 272, 34, 34 }, this);

		save_button = App->gui->addButton(900, 15, { 1084, 374, 27, 27 }, App->gui->atlas, nullptr, nullptr, { 1084, 374, 27, 27 }, { 1084, 374, 27, 27 }, this);

		if (current_map->next == nullptr)
		{
			moneda_1 = App->gui->addImageOnMap(933, 120, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_2 = App->gui->addImageOnMap(1600, 600, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_3 = App->gui->addImageOnMap(2700, 333, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_4 = App->gui->addImageOnMap(3400, 472, { 1060, 72, 33, 33 }, App->gui->atlas, this);
		}

		else
		{
			moneda_1 = App->gui->addImageOnMap(300, 530, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_2 = App->gui->addImageOnMap(1500, 156, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_3 = App->gui->addImageOnMap(3000, 382, { 1060, 72, 33, 33 }, App->gui->atlas, this);
			moneda_4 = App->gui->addImageOnMap(3700, 450, { 1060, 72, 33, 33 }, App->gui->atlas, this);
		}

		App->render->camera.x = App->render->cam.x = 0;
		App->render->camera.y = App->render->cam.y = 0;

		App->player->SpawnPLayer();
	}
	return true;
}

void j1Scene::CallBack(GUI_object* object, GUI_OBJECT_STATE state)
{
	switch (state) {
	case GUI_OBJECT_STATE::MOUSE_IN:

		break;
	case GUI_OBJECT_STATE::MOUSE_OUT:

		break;
	case GUI_OBJECT_STATE::MOUSE_ON_CLICK:

		break;
	case GUI_OBJECT_STATE::MOUSE_OFF_CLICK:
		if (object == gear)
		{
			open_menu = true;
		}

		else if (object == save_button)
			saveGame = true;

		break;
	}
}