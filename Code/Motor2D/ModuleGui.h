#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"

#include "SDL_image\include\SDL_image.h"

enum GUI_TYPES {
	GUI_OBJECT_NULL,
	IMAGE,
	BUTTON,
	LABEL,
	TEXT_BOX,
};

enum GUI_OBJECT_STATE {
	MOUSE_IN,
	MOUSE_OUT,
	MOUSE_ON_CLICK,
	MOUSE_OFF_CLICK
};

struct GUI_object {
	GUI_object();
	GUI_object(int x, int y, SDL_Rect rect, GUI_object* parent);
	~GUI_object();

	int x, y;
	SDL_Rect rect;
	GUI_TYPES type;
	GUI_OBJECT_STATE actualState = MOUSE_OUT;

	bool active = true, isMovable = true;
	GUI_object* parent;

	virtual void UpdateObject() = 0;
	virtual void Draw() = 0;
	void updatePosition();
	void changeState(GUI_OBJECT_STATE state);
	virtual void MouseInFunction() {};
	virtual void MouseOutFunction() {};
	virtual void MouseClikOnFunction() {};
	virtual void MouseClikOffFunction() {};

protected:
	int dToParentX;
	int dToParentY;
};

class ModuleGUI : public j1Module
{
public:

	ModuleGUI();
	virtual ~ModuleGUI();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	GUI_object* addImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent = nullptr);
	GUI_object* addButton(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr);
	GUI_object* addCheckBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr);
	//void addLabel(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, GUI_object* parent);
	//void addTextBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, GUI_object* parent);

	void mouseInteractionObjects();
private:
	p2List<GUI_object*> gui_objects;
	SDL_Texture* fons = nullptr;
	SDL_Texture* guiObjTextures = nullptr;
	//_TTF_Font* font;
};
#endif