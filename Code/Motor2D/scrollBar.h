#ifndef __gui_scrollBar_h__
#define __gui_scrollBar_h__

#include "ModuleGui.h"
#include "gui_image.h"

struct GUI_ScrollBar : public GUI_image {
	GUI_ScrollBar(int x, int y, SDL_Rect rectBase, SDL_Texture* texBase, SDL_Texture* texMovable, SDL_Rect rectMovable, int margeX, int margeY, bool blockX, j1Module* listener, GUI_object* parent);
	~GUI_ScrollBar();

	virtual void MouseClikOnFunction();
	virtual void MouseClikOffFunction();
	virtual void UpdateObject();
	virtual void Draw();

	void CleanUp();

	int returnNumer0to100();

	bool isMoving = false, blockX;		// si es false, es mou en eix X, sino en Y
	int margeX, margeY;

	virtual void MakeCallBack();

protected:
	GUI_image* pivoteMovable;
};

#endif