#include "gui_image.h"
#include "j1App.h"
#include "j1Render.h"

#include "j1Input.h"

GUI_image::GUI_image(int x, int y, SDL_Rect rect, SDL_Texture* tex, j1Module* listener, GUI_object* parent) : GUI_object(x,y,rect, listener, parent){
	this->texture = tex;
}

void GUI_image::UpdateObject() {
	updatePosition();
	Draw();
}

void GUI_image::Draw() {
	if (texture != nullptr)
		App->render->Blit(texture, App->render->camera.x * -1 + x, App->render->camera.y * -1 +  y, &rect);
}

void GUI_image::MouseClikOnFunction() {
	printf_s("S'ha clicat la imatge\n");
	CridaCallBack();
}

void GUI_image::CridaCallBack() {
	if (listener != nullptr)
		listener->CallBack((GUI_object*)this, actualState);
}