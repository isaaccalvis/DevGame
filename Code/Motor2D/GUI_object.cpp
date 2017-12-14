#include "GUI_object.h"

GUI_object::GUI_object() {}

GUI_object::GUI_object(int x, int y, SDL_Rect rect = { 0,0,0,0 }, GUI_object* parent = nullptr) {
	this->dToParentX = this->x = x;
	this->dToParentY = this->y = y;
	this->rect = rect;
	this->parent = parent;
}

GUI_object::~GUI_object() {

}

void GUI_object::updatePosition() {
	if (parent != nullptr) {
		this->x = dToParentX + parent->x;
		this->y = dToParentY + parent->y;
	}
}

void GUI_object::changeState(GUI_OBJECT_STATE state) {
	actualState = state;
	switch (state) {
	case GUI_OBJECT_STATE::MOUSE_IN:
		MouseInFunction();
		break;
	case GUI_OBJECT_STATE::MOUSE_OUT:
		MouseOutFunction();
		break;
	case GUI_OBJECT_STATE::MOUSE_ON_CLICK:
		MouseClikOnFunction();
		break;
	case GUI_OBJECT_STATE::MOUSE_OFF_CLICK:
		MouseClikOffFunction();
		break;
	}
}