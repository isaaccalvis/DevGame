#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Menu.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "j1FadeToBlack.h"

#include "ModuleGui.h"
#include "gui_image.h"

Menu::Menu() : j1Module(){
	name.create("scene");
}

Menu::~Menu(){}

bool Menu::Awake(pugi::xml_node& config){
	return true;
}

bool Menu::Start(){
	back = App->tex->Load("textures/menu_background.png");

	App->gui->atlas = App->tex->Load("textures/Game_Gui_Buttons.png");

	background = App->gui->addImage(0, 0, { 0, 0, 1024, 640 }, back, nullptr, nullptr, false);

	new_game = App->gui->addButton(170, 330, { 50, 25, 250, 70 }, App->gui->atlas, nullptr, nullptr, { 50, 212, 250, 70 }, { 50, 395, 250, 70 }, this);

	settings = App->gui->addButton(630, 160, { 50, 120, 250, 70 }, App->gui->atlas, nullptr, nullptr, { 50, 298, 250, 70 }, { 50, 490, 250, 70 }, this);

	credits = App->gui->addButton(630, 330, { 435, 122, 255, 75 }, App->gui->atlas, nullptr, nullptr, { 435, 300, 255, 75 }, { 435, 490, 255, 70 }, this);

	quit = App->gui->addButton(630, 500, { 435, 25, 255, 75 }, App->gui->atlas, nullptr, nullptr, { 435, 213, 255, 75 }, { 435, 393, 255, 75 }, this);

	load_button = App->gui->addButton(170, 500, { 735, 25, 250, 70 }, App->gui->atlas, nullptr, nullptr, { 735, 121, 250, 70 }, { 735, 215, 250, 70 }, this);

	return true;
}

bool Menu::PreUpdate(){
	return true;
}

bool Menu::Update(float dt){
	return true;
}

bool Menu::PostUpdate(){
	bool ret = true;

	if (start_game) {

		App->map->Start();

		App->fade->FadeToBlack(this, (j1Module*)App->scene, 1.0f);

		start_game = false;
	}

	else if (exit_game) {
		ret = false;
		exit_game = false;
	}

	else if (opt) {
		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data->CleanUp();
		}
		back = App->tex->Load("textures/menu_background.png");

		App->gui->atlas = App->tex->Load("textures/Game_Gui_Buttons.png");

		App->gui->gui_objects.clear();

		background = App->gui->addImage(0, 0, { 0, 0, 1024, 640 }, back, nullptr, nullptr, false);

		save_button = App->gui->addButton(170, 500, { 735, 300, 250, 70 }, App->gui->atlas, nullptr, nullptr, { 735, 395, 250, 70 }, { 735, 490, 250, 70 }, this);

		back_button = App->gui->addButton(630, 500, { 1220, 27, 255, 70 }, App->gui->atlas, nullptr, nullptr, { 1220, 125, 255, 70 }, { 1220, 215, 255, 70 }, this);

		opt = false;
	}

	else if (cred) {
		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data->CleanUp();
		}
		App->gui->gui_objects.clear();

		cred = false;
	}

	else if (loadGame) {
		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data->CleanUp();
		}
		App->gui->gui_objects.clear();

		App->enemies->active = true;
		App->scene->active = true;
		App->map->active = true;
		App->player->active = true;

		App->LoadGame();

		loadGame = false;
	}

	return ret;
}

bool Menu::CleanUp()
{
	LOG("Freeing menu");

	for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
	{
		it->data->CleanUp();
	}
	App->gui->gui_objects.clear();

	if (active) {

		App->enemies->Start();
		App->player->Start();

		App->enemies->active = true;
		App->scene->active = true;
		App->map->active = true;
		App->player->active = true;

		active = false;
	}

	return true;
}

void Menu::CallBack(GUI_object* object, GUI_OBJECT_STATE state) 
{
	switch (state) {
	case GUI_OBJECT_STATE::MOUSE_IN:

		break;
	case GUI_OBJECT_STATE::MOUSE_OUT:

		break;
	case GUI_OBJECT_STATE::MOUSE_ON_CLICK:

		break;
	case GUI_OBJECT_STATE::MOUSE_OFF_CLICK:
		if (object == new_game)
			start_game = true;

		else if (object == load_button)
			loadGame = true;

		else if (object == settings)
			opt = true;

		else if (object == credits)
			cred = true;

		else if (object == quit)
			exit_game = true;

		break;
	}
}