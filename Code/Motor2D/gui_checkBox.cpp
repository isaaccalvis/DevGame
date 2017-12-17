#include "gui_checkBox.h"

GUI_checkBox::GUI_checkBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, j1Module* listener = nullptr, GUI_object* parent = nullptr) : GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, listener, parent) {

}

void GUI_checkBox::UpdateObject() {
	updatePosition();
	Draw();
}

void GUI_checkBox::MouseClikOnFunction() {
	if (isClicking == true) {
		isClicking = false;
		textureToDraw = texture;
		rectToDraw = &rect;
	}
	else {
		isClicking = true;
		textureToDraw = textureOnClick;
		rectToDraw = &rectOnClick;
	}
}

void GUI_checkBox::CleanUp() {}