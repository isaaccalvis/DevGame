#include "ModuleGui.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "gui_image.h"
#include "gui_button.h"
#include "gui_checkBox.h"
#include "gui_label.h"
#include "gui_checkBox.h"
#include "gui_textBox.h"
#include "scrollBar.h"
#include "j1Scene.h"

#include "j1Render.h"
#include "ModulePlayer.h"

ModuleGUI::ModuleGUI() {}
ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::Start() {
	font = App->fonts->Load("fonts/open_sans/OpenSans-Regular.ttf", 12);

	return true;
}
bool ModuleGUI::PreUpdate() {
	mouseInteractionObjects();
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
	return true;
}

void ModuleGUI::mouseInteractionObjects() {
	int nx, ny;
	App->input->GetMousePosition(nx, ny);

	p2List_item<GUI_object*>* rec = gui_objects.start;
	while (rec != nullptr) {
		if (nx > rec->data->x && ny > rec->data->y && nx < (rec->data->x + rec->data->rect.w) && ny < (rec->data->y + rec->data->rect.h)) {
			if (App->input->GetMouseButtonDown(1)) {
				if (rec->data->actualState != GUI_OBJECT_STATE::MOUSE_ON_CLICK) {
					rec->data->changeState(GUI_OBJECT_STATE::MOUSE_ON_CLICK);
					if (focus == nullptr)
						focus = rec->data;
				}
			}
			else if (rec->data->actualState == GUI_OBJECT_STATE::MOUSE_ON_CLICK) {
				rec->data->changeState(GUI_OBJECT_STATE::MOUSE_OFF_CLICK);
				if (focus == rec->data)
					focus = nullptr; 
			}
			else if (rec->data->actualState != GUI_OBJECT_STATE::MOUSE_IN) {
				rec->data->changeState(GUI_OBJECT_STATE::MOUSE_IN);
				if (focus == rec->data)
					focus = nullptr;
			}
		}
		else if (rec->data->actualState != GUI_OBJECT_STATE::MOUSE_ON_CLICK && rec->data->actualState != GUI_OBJECT_STATE::MOUSE_OUT) {
			rec->data->changeState(GUI_OBJECT_STATE::MOUSE_OUT);
			if (focus == rec->data)
				focus = nullptr;
		}
		else if (!App->input->GetMouseButtonDown(1) && rec->data->actualState != GUI_OBJECT_STATE::MOUSE_OUT) {
			rec->data->changeState(GUI_OBJECT_STATE::MOUSE_OUT);
			if (focus == rec->data)
				focus = nullptr;
		}

		rec->data->isMoving = false;
		if (rec->data->actualState == GUI_OBJECT_STATE::MOUSE_ON_CLICK && App->scene->godmode == true)
			if (rec->data->type == GUI_TYPES::BUTTON_MOVABLE || rec->data->type == GUI_TYPES::IMAGE_MOVABLE || rec->data->type == GUI_TYPES::LABEL_MOVABLE || rec->data->type == GUI_TYPES::TEXT_BOX_MOVABLE || rec->data->type == GUI_TYPES::CHECK_BOX_MOVABLE)
				if (focus == rec->data)
					rec->data->isMoving = true;

		rec = rec->next;
	}
}

void ModuleGUI::setFocus(GUI_object* newFocus) {
	focus = newFocus;
}

GUI_object* ModuleGUI::addImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_image(x, y, rect, tex,listener, parent);
	if (isMovable == false)
		ret->type = IMAGE;
	else
		ret->type = IMAGE_MOVABLE;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addButton(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick,listener, parent);
	if (isMovable == false)
		ret->type = BUTTON;
	else
		ret->type = BUTTON_MOVABLE;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addCheckBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_checkBox(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick,listener, parent);
	if (isMovable == false)
		ret->type = CHECK_BOX;
	else
		ret->type = CHECK_BOX_MOVABLE;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addScrollBar(int x, int y, SDL_Rect rectBase, SDL_Texture* texBase, SDL_Texture* texMovable, SDL_Rect rectMovable, int margeX, int margeY, bool blockX, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_ScrollBar(x, y, rectBase, texBase, texMovable, rectMovable, margeX, margeY, blockX,listener, parent);
	ret->type = SCROLL_BAR;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addLabel(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_label(text, font, x, y, rect, color,listener, parent);
	if (isMovable == false)
		ret->type = LABEL;
	else
		ret->type = LABEL_MOVABLE;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addTextBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_textBox(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, text, font, rectLabel, color,listener, parent);
	if (isMovable == false)
		ret->type = TEXT_BOX;
	else
		ret->type = TEXT_BOX_MOVABLE;
	gui_objects.add(ret);
	return ret;
}

GUI_object* ModuleGUI::addImageOnMap(int x, int y, SDL_Rect rect, SDL_Texture* tex, j1Module* listener, GUI_object* parent, bool isMovable) {
	GUI_object* ret = new GUI_imageOnMap(x, y, rect, tex, listener, parent);
	if (isMovable == false)
		ret->type = IMAGE;
	else
		ret->type = IMAGE_MOVABLE;
	gui_objects.add(ret);
	return ret;
}