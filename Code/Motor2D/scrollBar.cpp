#include "j1App.h"
#include "j1Input.h"
#include "scrollBar.h"
#include "j1Render.h"

GUI_ScrollBar::GUI_ScrollBar(int x, int y, SDL_Rect rectBase, SDL_Texture* texBase, SDL_Texture* texMovable, SDL_Rect rectMovable, int margeX, int margeY, bool blockX = false, j1Module* listener = nullptr, GUI_object* parent = nullptr) : GUI_image(x, y, rectBase, texBase, listener, parent) {
	pivoteMovable = new GUI_image(x + margeX, y + margeY, rectMovable, texMovable, listener);
	this->margeX = margeX;
	this->margeY = margeY;
	this->blockX = blockX;
}

GUI_ScrollBar::~GUI_ScrollBar() {
	delete pivoteMovable;
}

void GUI_ScrollBar::MouseClikOnFunction() {
	int nx, ny;
	App->input->GetMousePosition(nx, ny);
	if (nx > pivoteMovable->x && nx < pivoteMovable->x + pivoteMovable->rect.w && ny > pivoteMovable->y && ny < pivoteMovable->y + pivoteMovable->rect.h && App->input->GetMouseButtonDown(1)) {
		App->gui->setFocus(pivoteMovable);
		isMoving = true;
	}
	if (isMoving == true) {
		if (blockX == false) {			// ES MOU EN L'EIX X

			if (App->input->GetMouseButtonDown(1)) {
				pivoteMovable->x = nx - pivoteMovable->rect.w / 2;

				if (pivoteMovable->x < x + margeX) {
					App->gui->setFocus(nullptr);
					isMoving = false;
					pivoteMovable->x = x + margeX;
				}
				if (pivoteMovable->x > x + rect.w - margeX) {
					App->gui->setFocus(nullptr);
					isMoving = false;
					pivoteMovable->x = x + rect.w - margeX;
				}
			}
			else {
				App->gui->setFocus(nullptr);
				isMoving = false;
			}
		}
		else {
			if (App->input->GetMouseButtonDown(1)) {
				pivoteMovable->y = ny - pivoteMovable->rect.h / 2;

				if (pivoteMovable->y < y + margeY) {
					App->gui->setFocus(nullptr);
					isMoving = false;
					pivoteMovable->y = y + margeY;
				}
				if (pivoteMovable->y > y + rect.h - + margeY) {
					App->gui->setFocus(nullptr);
					isMoving = false;
					pivoteMovable->y = y + rect.h - + margeY;
				}
			}
			else {
				App->gui->setFocus(nullptr);
				isMoving = false;
			}
		}
	}
}

void GUI_ScrollBar::UpdateObject() {
	MouseClikOnFunction();
	updatePosition();
	Draw();
}

void GUI_ScrollBar::Draw() {
	if (texture != nullptr)
		App->render->Blit(texture, App->render->camera.x * -1 + x, App->render->camera.y * -1 + y, &rect);
	if (pivoteMovable->texture != nullptr)
		App->render->Blit(pivoteMovable->texture, App->render->camera.x * -1 + pivoteMovable->x, App->render->camera.y * -1 + pivoteMovable->y, &pivoteMovable->rect);
}

int GUI_ScrollBar::returnNumer0to100() {
	if (blockX == false) {
		int totalDistance = rect.w - margeX * 2;
		int actualPositionRelative = pivoteMovable->x - x - margeX;
		int result = (actualPositionRelative * 100) / totalDistance;
		return result;
	}
	else {
		int totalDistance = rect.h - margeY * 2;
		int actualPositionRelative = pivoteMovable->y - y - margeY;
		int result = (actualPositionRelative * 100) / totalDistance;
		return result;
	}
}

void GUI_ScrollBar::CridaCallBack() {
	if (listener != nullptr)
		listener->CallBack((GUI_object*)this, actualState);
}