#ifndef __gui_button_h__
#define __gui_button_h__

#include "ModuleGui.h"
#include "gui_image.h"

struct GUI_button : public GUI_image {
	GUI_button(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr);
	SDL_Texture* textureOnMouse;
	SDL_Texture* textureOnClick;
	SDL_Rect rectOnMouse;
	SDL_Rect rectOnClick;
	virtual void UpdateObject();
	void Draw(SDL_Texture* tex, SDL_Rect rect);
	bool MouseOnClick();
};

#endif