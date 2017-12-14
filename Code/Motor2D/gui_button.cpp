#include "gui_button.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"

GUI_button::GUI_button(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, GUI_object* parent) : GUI_image(x, y, rect, tex, parent) {
	if (texOnMouse == nullptr)
		textureOnMouse = tex;
	else
		textureOnMouse = texOnMouse;

	if (texOnClick == nullptr)
		textureOnClick = tex;
	else
		textureOnClick = texOnClick;

	if (rectOnMouse.x == -1)
		this->rectOnMouse = rect;
	else
		this->rectOnMouse = rectOnMouse;

	if (rectOnClick.x == -1)
		this->rectOnClick = rect;
	else
		this->rectOnClick = rectOnClick;


	textureToDraw = texture;
	rectToDraw = &this->rect;
}

void GUI_button::UpdateObject() {
	printf_s("%i %i %i %i \n", rectToDraw->x, rectToDraw->y, rectToDraw->w, rectToDraw->h);
	updatePosition();
	Draw();
}

void GUI_button::Draw() {
	if (textureToDraw != nullptr) {
		App->render->Blit(textureToDraw, App->render->camera.x * -1 + x, App->render->camera.y * -1 + y, rectToDraw);
	}
}

void GUI_button::MouseInFunction() {
	textureToDraw = textureOnMouse;
	rectToDraw = &rectOnMouse;
	printf_s("mouse in \n");
}
void GUI_button::MouseOutFunction() {
	textureToDraw = texture;
	rectToDraw = &rect;
	printf_s("mouse out \n");
}
void GUI_button::MouseClikOnFunction() {
	textureToDraw = textureOnClick;
	rectToDraw = &rectOnClick;
	printf_s("mouse click \n");
}
void GUI_button::MouseClikOffFunction(){
	textureToDraw = textureOnMouse;
	rectToDraw = &rectOnMouse;
	printf_s("mouse off click \n");
}