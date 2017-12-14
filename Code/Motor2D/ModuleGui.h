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

struct GUI_object {
	GUI_object();
	GUI_object(int x, int y, SDL_Rect rect, GUI_object* parent);
	~GUI_object();
	int x, y;
	SDL_Rect rect;
	GUI_TYPES type;
	bool active = true;
	GUI_object* parent;

	virtual void UpdateObject() = 0;
	virtual void Draw() = 0;
	bool MouseOn();
	void updatePosition();

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
	bool PostUpdate();
	bool CleanUp();

	GUI_object* addImage(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent = nullptr);
	GUI_object* addButton(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr);
	GUI_object* addCheckBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, GUI_object* parent = nullptr);
	//void addLabel(char* text, _TTF_Font* font, int x, int y, SDL_Rect rect, SDL_Color color, GUI_object* parent);
	//void addTextBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse, SDL_Texture* texOnClick, SDL_Rect rectOnMouse, SDL_Rect rectOnClick, char* text, _TTF_Font* font, SDL_Rect rectLabel, SDL_Color color, GUI_object* parent);
private:
	p2List<GUI_object*> gui_objects;
	SDL_Texture* fons = nullptr;
	SDL_Texture* guiObjTextures = nullptr;
	//_TTF_Font* font;
};
#endif