#ifndef __gui_image_h__
#define __gui_image_h__

#include "ModuleGui.h"
#include "GUI_object.h"

struct GUI_image : public GUI_object {
	GUI_image(int x, int y, SDL_Rect rect, SDL_Texture* tex, j1Module* listener, GUI_object* parent = nullptr);
	virtual void UpdateObject();
	virtual void Draw();

	virtual void MouseClikOnFunction();
	virtual void CridaCallBack();
	SDL_Texture* texture;
};

#endif