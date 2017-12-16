#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Fonts.h"

#include "SDL_image\include\SDL_image.h"

enum GUI_TYPES {
	GUI_OBJECT_NULL,
	IMAGE,
	IMAGE_MOVABLE,
	BUTTON,
	BUTTON_MOVABLE,
	CHECK_BOX,
	CHECK_BOX_MOVABLE,
	LABEL,
	LABEL_MOVABLE,
	TEXT_BOX,
	TEXT_BOX_MOVABLE,
	SCROLL_BAR,
};

enum GUI_OBJECT_STATE {
	MOUSE_IN,
	MOUSE_OUT,
	MOUSE_ON_CLICK,
	MOUSE_OFF_CLICK
};

struct GUI_object;

class ModuleGUI : public j1Module
{
public:

	ModuleGUI();
	virtual ~ModuleGUI();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	GUI_object* addImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent = nullptr, bool isMovable = false);
	GUI_object* addButton(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr, bool isMovable = false);
	GUI_object* addCheckBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr, bool isMovable = false);
	GUI_object* addScrollBar(int x, int y, SDL_Rect rectBase, SDL_Texture* texBase, SDL_Texture* texMovable, SDL_Rect rectMovable, int margeX, int margeY, bool blockX, GUI_object* parent);
	GUI_object* addLabel(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, GUI_object* parent , bool isMovable);
	GUI_object* addTextBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, GUI_object* parent, bool isMovable);

	void mouseInteractionObjects(); 
	void setFocus(GUI_object* newFocus);
private:
	SDL_Texture* fons = nullptr;
	SDL_Texture* guiObjTextures = nullptr;
	_TTF_Font* font;

	GUI_object* focus = nullptr;

public:
	p2List<GUI_object*> gui_objects;
};
#endif