#include "gui_label.h"
#include "j1App.h"
#include "j1Render.h"

//GUI_label::GUI_label(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, GUI_object* parent) : GUI_object(x,y,rect,parent) {
//	this->text = text;
//	this->font = font;
//	this->color = color;
//}
//
//void GUI_label::Draw() {
//	if (font != nullptr && text != nullptr)
//		App->render->Blit(App->font->Print(text, color, font), x, y, &rect);
//}
//
//void GUI_label::UpdateObject() {
//	Draw();
//}