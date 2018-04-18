#include "GuiElement.h"
#include "Application.h"
#include "M_Gui.h"
#include "M_Render.h"
#include "M_Window.h"

GuiElement::GuiElement() {};

GuiElement::~GuiElement() {};

void GuiElement::Start() {};

void GuiElement::Update(float dt)
{

};

void GuiElement::Draw() {};

void GuiElement::OnClick() {};

void GuiElement::PositionUpdate()
{
	if (follows_camera == true)
	{
		iPoint cam = App->render->CameraToMapPosition();

		position.x = cam.x + 1;
		position.y = cam.y + 1;
	}
}