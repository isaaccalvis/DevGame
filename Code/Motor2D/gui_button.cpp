#include "gui_button.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"

GUI_button::GUI_button(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, GUI_object* parent) : GUI_image(x, y, rect, tex) {
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
}

bool GUI_button::MouseOnClick() {
	if (MouseOn() == true && App->input->GetMouseButtonDown(1))
		return true;
	return false;
}

void GUI_button::UpdateObject() {
	if (MouseOnClick())
		Draw(textureOnClick, rectOnClick);
	else if (MouseOn())
		Draw(textureOnMouse, rectOnMouse);
	Draw(texture, rect);
}

void GUI_button::Draw(SDL_Texture* tex, SDL_Rect rect) {
	if (texture != nullptr) {
		App->render->Blit(tex, x, y, &rect);
	}
}