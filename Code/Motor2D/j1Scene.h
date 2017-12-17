#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();
	virtual ~j1Scene();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool ChangeScene();

	void CallBack(GUI_object* object, GUI_OBJECT_STATE state);

private:
	SDL_Texture* debug_tex = nullptr;

	GUI_object* gear = nullptr;

	GUI_object* save_button = nullptr;

	GUI_object* moneda_1 = nullptr;
	GUI_object* moneda_2 = nullptr;
	GUI_object* moneda_3 = nullptr;
	GUI_object* moneda_4 = nullptr;

	bool open_menu = false;

	bool saveGame = false;
	
public:
	p2List<p2SString> maps;
	p2List_item<p2SString>* current_map;

	bool godmode = false;
};

#endif // __j1SCENE_H__