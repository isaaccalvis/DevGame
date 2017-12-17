#ifndef __gui_label_h__
#define __gui_label_h__

#include "ModuleGui.h"
#include "GUI_object.h"

struct GUI_label : public GUI_object {
	GUI_label(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, j1Module* listener, GUI_object* parent = nullptr);
	virtual void UpdateObject();
	virtual void Draw();
	char* text;
	_TTF_Font* font;
	SDL_Color color;

	virtual void CridaCallBack();

};

#endif