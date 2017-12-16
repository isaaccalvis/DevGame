#include "gui_image.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

#include "j1Input.h"

GUI_image::GUI_image(int x, int y, SDL_Rect rect, SDL_Texture* tex, GUI_object* parent) : GUI_object(x,y,rect,parent){
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

void GUI_image::CleanUp() {
	App->tex->UnLoad(texture);
}