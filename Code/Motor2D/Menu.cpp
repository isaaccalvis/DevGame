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
	texturaA = App->tex->Load("textures/wowBCscreen.jpg");
	imatgeA = App->gui->addImage(300, 400, SDL_Rect{ 0,0,100, 100 }, texturaA, nullptr, nullptr, true);
	buttonA = App->gui->addButton(500, 400, SDL_Rect{ 0,0,100,100 }, texturaA, nullptr, nullptr, SDL_Rect{200,0,100,100}, SDL_Rect{100,0,100,100}, this, nullptr, false);
	return true;
}

bool Menu::PreUpdate(){
	return true;
}

bool Menu::Update(float dt){
	return true;
}

bool Menu::PostUpdate(){
	return true;
}

bool Menu::CleanUp(){
	return true;
}

void Menu::CallBack(GUI_object* object, GUI_OBJECT_STATE state) {
	switch (state) {
	case GUI_OBJECT_STATE::MOUSE_IN:
		if (object == imatgeA)
			printf_s("a\n");
		break;
	case GUI_OBJECT_STATE::MOUSE_OUT:

		break;
	case GUI_OBJECT_STATE::MOUSE_ON_CLICK:
		if (object == imatgeA)
			printf_s("b\n");
		else if (object == buttonA)
			printf_s("button detected\n");
		break;
	case GUI_OBJECT_STATE::MOUSE_OFF_CLICK:

		break;
	}
}