#ifndef __Menu_H__
#define __Menu_H__

#include "j1Module.h"
#include "ModuleGui.h"

class Menu : public j1Module
{
public:

	Menu();
	virtual ~Menu();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void CallBack(GUI_object* object, GUI_OBJECT_STATE state);

public:
	GUI_object* imatgeA = nullptr;
	GUI_object* buttonA = nullptr;
	SDL_Texture* texturaA = nullptr;
};

#endif