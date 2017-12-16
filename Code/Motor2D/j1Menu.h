#ifndef __j1MENU_H__
#define __j1MENU_H__

#include "j1Module.h"

struct GUI_image;
struct GUI_button;
struct GUI_textBox;
struct GUI_label;
struct GUI_checkBox;

class j1Menu : public j1Module
{
public:

	j1Menu();
	virtual ~j1Menu();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void GUI_CallBack(GUI_object* object);

private:

	GUI_image* background = nullptr;

	GUI_button* new_game = nullptr;
	GUI_button* settings = nullptr;
	GUI_button* credits = nullptr;
	GUI_button* quit = nullptr;

	bool start_game = false;
	bool exit = false;
	bool options = false;
	bool cred = false;
	SDL_Texture* back = nullptr;

	SDL_Texture* buttons = nullptr;
}

#endif // __j1MENU_H__