#include "GUI_object.h"
#include "j1Input.h"

GUI_object::GUI_object() {}

GUI_object::GUI_object(int x, int y, SDL_Rect rect = { 0,0,0,0 }, j1Module* listener = nullptr, GUI_object* parent = nullptr) {
	this->dToParentX = this->x = x;
	this->dToParentY = this->y = y;
	this->rect = rect;
	this->parent = parent;
	this->listener = listener;
	this->atlas = App->gui->atlas;
}

GUI_object::~GUI_object() {

}

void GUI_object::updatePosition() {
	moveWithMouse();
	if (parent != nullptr) {
		this->x = dToParentX + parent->x;
		this->y = dToParentY + parent->y;
	}
}

void GUI_object::moveWithMouse() {
	int nx, ny;
	App->input->GetMousePosition(nx, ny);
	if (isMoving == true) {
		if (dToMouseX == -1) {
			if (parent != nullptr) {
				dToMouseX = nx - x;
				dToMouseY = ny - y;
			}
			else {
				dToMouseX = nx - x;
				dToMouseY = ny - y;
			}
		}
		else {
			if (parent != nullptr) {
				dToParentX = nx - x;
				dToParentY = ny - y;
			}
			else {
				x = nx - dToMouseX;
				y = ny - dToMouseY;
			}
		}
	}
	else {
		if (dToMouseX != -1) {
			dToMouseX = -1;
			dToMouseY = -1;
		}
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

void GUI_object::MakeCallBack() {
	listener->CallBack((GUI_object*)this, actualState);
}