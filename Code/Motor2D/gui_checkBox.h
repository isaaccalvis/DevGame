#ifndef __gui_checkBox_h__
#define __gui_checkBox_h__

#include "ModuleGui.h"
#include "gui_button.h"

struct GUI_checkBox : public GUI_button {
	GUI_checkBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent);
	SDL_Texture* texActual = nullptr;
	SDL_Rect rectActual = { 0,0,0,0 };
	bool isClicking = false;
	void UpdateObject();
};

#endif