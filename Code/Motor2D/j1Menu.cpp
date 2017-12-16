#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Menu.h"
#include "ModuleGui.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "GUI_object.h"
#include "gui_image.h"


j1Menu::j1Menu() : j1Module()
{
	name.create("menu");
}

// Destructor
j1Menu::~j1Menu()
{}

bool j1Menu::Awake(pugi::xml_node& config)
{
	LOG("Loading Menu");
	bool ret = true;

	return ret;
}


bool j1Menu::Start()
{
	back = App->tex->Load("textures/menu_background.png");

	buttons = App->tex->Load("textures/menu_buttons.png");

	background = (GUI_image*)App->gui->addImage(0, 0, { 0, 0, 1024, 640 }, back, NULL, false);

	new_game = (GUI_button*)App->gui->addButton(170, 330, { 50, 25, 250, 70 }, buttons, buttons, buttons, { 50, 120, 250, 70 }, { 50, 395, 250, 70 }, NULL);

	settings = (GUI_button*)App->gui->addButton(170, 500, { 50, 120, 250, 70 }, buttons, buttons, buttons, { 50, 300, 250, 70 }, { 50, 490, 250, 70 }, NULL);

	//credits = (GUI_button*)App->gui->addButton(170, 330, { 50, 25, 250, 70 }, buttons, buttons, buttons, { 50, 120, 250, 70 }, { 50, 395, 250, 70 }, NULL);

	quit = (GUI_button*)App->gui->addButton(630, 500, { 435, 25, 255, 75 }, buttons, buttons, buttons, { 435, 215, 255, 75 }, { 435, 393, 255, 75 }, NULL);

	return true;
}


bool j1Menu::PreUpdate()
{
	return true;
}

bool j1Menu::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Menu::PostUpdate()
{
	bool ret = true;

	if (start_game) {
		App->fade->FadeToBlack(this, (j1Module*)App->scene, 1.0f);

		App->scene->active = true;
		App->map->active = true;
		App->player->active = true;
		App->enemies->active = true;

		App->map->Start();
		App->player->Start();
		App->enemies->Start();
		start_game = false;
	}

	else if (exit) {
		ret = false;
		exit = false;
	}

	else if (options) {
		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data;
		}
		App->gui->gui_objects.clear();

		options = false;
	}

	else if (cred) {
		for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
		{
			it->data->CleanUp();
		}
		App->gui->gui_objects.clear();

		credits = false;
	}

	return ret;
}

// Called before quitting
bool j1Menu::CleanUp()
{
	LOG("Freeing menu");

	for (p2List_item<GUI_object*>* it = App->gui->gui_objects.start; it != nullptr; it = it->next)
	{
		it->data->CleanUp();
	}
	App->gui->gui_objects.clear();

	return true;
}

void j1Menu::GUI_CallBack(GUI_object* object)
{
	if (new_game == object)
		start_game = true;

	else if (settings == object)
		options = true;

	else if (credits == object)
		cred = true;

	else if (quit == object)
		exit = true;
}