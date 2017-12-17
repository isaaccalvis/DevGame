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

	GUI_object* background = nullptr;

	GUI_object* new_game = nullptr;
	GUI_object* settings = nullptr;
	GUI_object* credits = nullptr;
	GUI_object* quit = nullptr;
	GUI_object* save_button = nullptr;
	GUI_object* load_button = nullptr;
	GUI_object* back_button = nullptr;

	bool start_game = false;
	bool exit_game = false;
	bool opt = false;
	bool cred = false;
	bool loadGame = false;
	bool need_load = false;
	SDL_Texture* back = nullptr;
};

#endif