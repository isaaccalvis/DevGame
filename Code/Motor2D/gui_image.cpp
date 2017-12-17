#include "gui_image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

#include "j1Input.h"
#include "j1Map.h"

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
	MakeCallBack();
}

void GUI_image::MakeCallBack() {
	if (listener != nullptr)
		listener->CallBack((GUI_object*)this, actualState);
}

void GUI_image::CleanUp() {
	App->tex->UnLoad(texture);
}

GUI_imageOnMap::GUI_imageOnMap(int x, int y, SDL_Rect rect, SDL_Texture* tex, j1Module* listener, GUI_object* parent) : GUI_image(x,y,rect,tex,listener,parent){
	realX = x;
}

void GUI_imageOnMap::UpdateObject() {
	if (isActivated == true) {
		Draw();
		x = realX + (int)App->render->cam.x;
	}
}

void GUI_imageOnMap::Draw() {
	App->render->Blit(texture, realX, y, &rect);
}

void GUI_imageOnMap::MouseClikOnFunction() {
	if (isActivated == true) {
		isActivated = false;
	}
}