#include "gui_label.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"

GUI_label::GUI_label(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, j1Module* listener, GUI_object* parent) : GUI_object(x,y,rect,listener,parent) {
	this->text = text;
	this->font = font;
	this->color = color;
}

void GUI_label::Draw() {
	if (font != nullptr && text != nullptr)
		App->render->Blit(App->fonts->Print(text, color, font), App->render->camera.x * -1 + x, App->render->camera.y * -1 + y, &this->rect);
}

void GUI_label::UpdateObject() {
	updatePosition();
	Draw();
}

void GUI_label::CleanUp() {}

void GUI_label::MakeCallBack() {
	if (listener != nullptr)
		listener->CallBack((GUI_object*)this, actualState);
}