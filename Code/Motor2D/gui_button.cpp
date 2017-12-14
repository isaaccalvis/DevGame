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
}

void GUI_button::UpdateObject() {
	updatePosition();
	Draw(texture, rect);
}

void GUI_button::Draw(SDL_Texture* tex, SDL_Rect rect) {
	if (textureToDraw != nullptr) {
		App->render->Blit(textureToDraw, App->render->camera.x * -1 + x, App->render->camera.y * -1 + y, &rect);
	}
}

void GUI_button::MouseInFunction() {
	textureToDraw = textureOnMouse;
	printf_s("mouse in \n");
}
void GUI_button::MouseOutFunction() {
	textureToDraw = texture;
	printf_s("mouse out \n");
}
void GUI_button::MouseClikOnFunction() {
	textureToDraw = textureOnClick;
	printf_s("mouse click \n");
}
void GUI_button::MouseClikOffFunction(){
	textureToDraw = textureOnMouse;
	printf_s("mouse off click \n");
}