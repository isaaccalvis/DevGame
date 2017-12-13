#include "gui_checkBox.h"

GUI_checkBox::GUI_checkBox(int x, int y, SDL_Rect rect, SDL_Texture* tex, SDL_Texture* texOnMouse = nullptr, SDL_Texture* texOnClick = nullptr, SDL_Rect rectOnMouse = SDL_Rect{ -1,0,0,0 }, SDL_Rect rectOnClick = SDL_Rect{ -1,0,0,0 }, GUI_object* parent = nullptr) : GUI_button(x, y, rect, tex, texOnMouse, texOnClick, rectOnMouse, rectOnClick) {
	texActual = texture;
	rectActual = rect;
}

void GUI_checkBox::UpdateObject() {
	if (MouseOnClick()) {
		if (isClicking == false)
			if (texActual == texture) {
				texActual = textureOnClick;
				rectActual = rectOnClick;
			}
			else {
				texActual = texture;
				rectActual = rect;
			}
			isClicking = true;
	}
	else
		isClicking = false;
	if (MouseOn())
		Draw(textureOnMouse, rectOnMouse);
	Draw(texActual, rectActual);
}