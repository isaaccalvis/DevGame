#ifndef __gui_image_h__
#define __gui_image_h__

#include "ModuleGui.h"

struct GUI_image : public GUI_object {
	GUI_image(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent = nullptr);
	SDL_Texture* texture;
	virtual void UpdateObject();
	virtual void Draw();
};

#endif