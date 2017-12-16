#ifndef __GUIobject_H__
#define __GUIobject_H__

#include "ModuleGui.h"

struct GUI_object {
	GUI_object();
	GUI_object(int x, int y, SDL_Rect rect, GUI_object* parent);
	~GUI_object();

	int x, y;
	SDL_Rect rect;
	GUI_TYPES type;
	GUI_OBJECT_STATE actualState = MOUSE_OUT;

	bool active = true, isMoving = true;
	GUI_object* parent;

	virtual void UpdateObject() = 0;
	virtual void Draw() = 0;
	void updatePosition();
	void moveWithMouse();
	void changeState(GUI_OBJECT_STATE state);

	virtual void CleanUp() {};
	virtual void MouseInFunction() {};
	virtual void MouseOutFunction() {};
	virtual void MouseClikOnFunction() {};
	virtual void MouseClikOffFunction() {};

protected:
	int dToParentX = 0;
	int dToParentY = 0;

	int dToMouseX = -1;
	int dToMouseY = -1;
};
#endif
