#include "j1App.h"
#include "j1Input.h"
#include "gui_textBox.h"

GUI_textBox::GUI_textBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, j1Module* listener, GUI_object* parent = nullptr) : GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, listener,parent) {
	textLabel = new GUI_label (text, font, x, y, rectLabel, color, listener);
}

void GUI_textBox::UpdateObject() {
	updatePosition();
	Draw();
}

void GUI_textBox::Draw() {

}

void GUI_textBox::deletLabelInside() {
	delete textLabel;
}

void GUI_textBox::CleanUp() {}