#ifndef __gui_textBox_h__
#define __gui_textBox_h__

#include "ModuleGui.h"
#include "gui_label.h"
#include "gui_button.h"

struct GUI_textBox : public GUI_button {
	GUI_textBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, j1Module* listener, GUI_object* parent);

	int posYbar = 0;
	void Draw();
	bool activated = false;
	void UpdateObject();
	GUI_label *textLabel;

	void deletLabelInside();
};

#endif