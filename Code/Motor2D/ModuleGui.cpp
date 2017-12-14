#include "ModuleGui.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "gui_image.h"
#include "gui_button.h"
#include "gui_checkBox.h"
#include "gui_label.h"
#include "gui_checkBox.h"
#include "gui_textBox.h"

#include "j1Render.h"
#include "ModulePlayer.h"

ModuleGUI::ModuleGUI() {}
ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::Start() {
	guiObjTextures = App->tex->Load("textures/objectesGUI.png");
	fons = App->tex->Load("textures/wowBCscreen.jpg");
	GUI_object* papi = addImage(0, 0, SDL_Rect{ 0,0,100, 100 }, guiObjTextures);
	addButton(10, 20, SDL_Rect{ 20,530,120,20 }, fons, guiObjTextures, guiObjTextures, SDL_Rect{ 0,0,140,50 }, SDL_Rect{ 160, 0, 140, 50 }, papi);
	addCheckBox(300, 30, SDL_Rect{ 20,530,120,20 }, fons, guiObjTextures, guiObjTextures, SDL_Rect{ 0,0,140,50 }, SDL_Rect{ 160, 0, 140, 50 }, papi);

	return true;
}

bool ModuleGUI::PostUpdate() {
	p2List_item<GUI_object*>* rec = gui_objects.start;
	while (rec != nullptr) {
		rec->data->UpdateObject();
		rec = rec->next;
	}
	return true;
}

bool ModuleGUI::CleanUp() {
	App->tex->UnLoad(guiObjTextures);
	return true;
}

GUI_object* ModuleGUI::addImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent) {
	GUI_object* ret = new GUI_image(x, y, rect, tex, parent);
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addButton(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, GUI_object* parent) {
	GUI_object* ret = new GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, parent);
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addCheckBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, GUI_object* parent) {
	GUI_object* ret = new GUI_checkBox(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, parent);
	gui_objects.add(ret);
	return ret;
}

//void ModuleGUI::addLabel(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, GUI_object* parent = nullptr) {
//	GUI_object* ret = new GUI_label(text, font, x, y, rect, color, parent);
//	gui_objects.add(ret);
//}
//
//void ModuleGUI::addTextBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, GUI_object* parent = nullptr) {
//	GUI_object* ret = new GUI_textBox(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, text, font, rectLabel, color, parent);
//	gui_objects.add(ret);
//}

//--------------------------------------------- GUI_object ---------------------------------------------

GUI_object::GUI_object() {}

GUI_object::GUI_object(int x, int y, SDL_Rect rect = {0,0,0,0}, GUI_object* parent = nullptr) {
	this->dToParentX = this->x = x;
	this->dToParentY = this->y = y;
	this->rect = rect;
	//this->type = type;
	this->parent = parent;
}

GUI_object::~GUI_object() {

}

bool GUI_object::MouseOn() {
	int nx, ny;
	App->input->GetMousePosition(nx, ny);
	if (nx > this->x && ny > this->y && nx < (this->x + rect.w) && ny < (this->y + rect.h))
		return true;
	return false;
}

void GUI_object::updatePosition() {
	if (parent != nullptr) {
		this->x = dToParentX + parent->x;
		this->y = dToParentY + parent->y;
	}
}