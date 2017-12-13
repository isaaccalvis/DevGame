#include "gui_textBox.h"
#include "j1App.h"
#include "j1Input.h"

//GUI_textBox::GUI_textBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color) : GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick, GUI_object* parent) {
//	textLabel = new GUI_label (text, font, x, y, rectLabel, color);
//}
//
//void GUI_textBox::UpdateObject() {
//	if (App->input->GetMouseButtonDown(1))
//		if (MouseOnClick())
//			activated = true;
//		else
//			activated = false;
//	else
//		activated = false;
//	if (activated)
//		printf_s("a");
//	Draw();
//
//}
//
//void GUI_textBox::Draw() {
//
//}
//
//void GUI_textBox::deletLabelInside() {
//	delete textLabel;
//}